/**
    PiCaster is a podcast recording and streaming application,
    easy to use for beginners, but with enough functionality
    to suit the needs of intermediate or experienced podcasters.
    Copyright (C) 2018  Yannick Mauray
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

namespace LibAudio
{ 
    public class JackClient
    {
        private Jack.Client client;

        private JackModule[] modules = {};

        private Posix.pid_t server_pid;

        public JackClient()
        {
        }

        public unowned Jack.Client get_client()
        {
            return this.client;
        }

        public void start()
        {
            if (this.client != null) return;
            Jack.Options options = Jack.Options.JackNoStartServer;
            Jack.Status status;
            this.client = Jack.Client.open("PiCaster", options, out status);
            if (this.client == null)
            {
                var jackd_path = Common.Settings.get_string("jack", "jackd_path_entry", "/usr/bin/jackd");
                var driver_name = Common.Settings.get_string("jack", "driver", "ALSA");
                var periods_per_buffer = Common.Settings.get_string("jack", "periods_per_buffer", "3");
                var input_device = Common.Settings.get_string("jack", "input_device_id", "");
                var output_device = Common.Settings.get_string("jack", "output_device_id", "");
                var frames_per_period = Common.Settings.get_string("jack", "frames_per_period", "256");
                var sample_rate = Common.Settings.get_string("jack", "sample_rate", "48000");
                string[] params = {
                    jackd_path,
                    "-d" + driver_name,
                    "-Chw:" + input_device,
                    "-Phw:" + output_device,
                    "-p" + frames_per_period,
                    "-r" + sample_rate,
                };
                if (driver_name != "dummy")
                {
                    params += "-n" + periods_per_buffer;
                }
                
                // Fork jackd
                int server_pid = Posix.fork();
                if (server_pid == 0)
                {
                    // child
                    Posix.setsid();
                    stderr.printf("%s %s %s %s %s %s %s\n", params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
                    Posix.execv(params[0], params);
                }
                else
                {
                    // parent
                    GLib.Thread.usleep(500000);
                    this.server_pid = server_pid;
                    Posix.signal(Posix.SIGCHLD, Posix.SIG_IGN);
                    this.client = Jack.Client.open("PiCaster", options, out status);
                }
            }
            this.client.set_xrun_callback(() => {
                stderr.printf("JACK XRun ...\n");
                return 0;
            });
            this.client.on_shutdown(() => {
                stderr.printf("Jack server has gone away...\n");
                //exit(1);
            });
            this.client.set_process_callback((nframes) => {
                foreach(var module in modules)
                {
                    module.process(nframes);
                }
                return 0;
            });

            Posix.sighandler_t signal_handler = (sig) => {
                stderr.printf("Signal received : %d\n", sig);
                PiCaster.App.jack_client.stop();
                exit(1);
            };

            Posix.signal(Posix.SIGQUIT, signal_handler);
            Posix.signal(Posix.SIGHUP, signal_handler);
            Posix.signal(Posix.SIGTERM, signal_handler);
            Posix.signal(Posix.SIGINT, signal_handler);

            this.client.activate();
            this.init_modules();
        }

        public void stop()
        {
            if (this.server_pid > 0)
            {
                Posix.kill(this.server_pid, Posix.SIGKILL);
                this.server_pid = 0;
            }
            this.client = null;
        }

        public void init_modules()
        {
            JackPorts playback_ports = this.get_playback_ports();

            var recorder = new JackRecorderModule("recorder", this);
            this.register_module(recorder);

            var master_fader = new JackFaderModule("master_fader", this);
            this.register_module(master_fader);
            master_fader.connect_to_ports(playback_ports);
            master_fader.connect_to_module(recorder);
            master_fader.set_active(true);
            PiCaster.App.bus.master_fader_changed.connect(master_fader.set_slider_value);

            var dj_fader = new JackFaderModule("dj_fader", this);
            this.register_module(dj_fader);
            dj_fader.connect_to_module(master_fader);
            PiCaster.App.bus.dj_fader_changed.connect(dj_fader.set_slider_value);
            PiCaster.App.bus.mic_button_active.connect(dj_fader.set_active);

            var track_fader = new JackFaderModule("track_fader", this);
            this.register_module(track_fader);
            track_fader.connect_to_module(master_fader);
            track_fader.set_active(true);
            PiCaster.App.bus.track_fader_changed.connect(track_fader.set_slider_value);

            var deck_a = new JackFilePlayerModule("deck_a", this);
            this.register_module(deck_a);
            deck_a.index = 0;
            deck_a.connect_to_module(track_fader);

            var deck_b = new JackFilePlayerModule("deck_b", this);
            this.register_module(deck_b);
            deck_a.index = 1;
            deck_b.connect_to_module(track_fader);

            JackFilePlayerModule[] decks = {deck_a, deck_b};

            PiCaster.App.bus.play_track.connect((filename) => {
                var deck = deck_a.is_playing() ? deck_b : deck_a;
                deck.play(filename);
                return deck.index;
            });
            PiCaster.App.bus.stop_track.connect((player_index) => {
                var deck = decks[player_index];
                deck.stop();
            });

            JackPorts capture_ports = this.get_capture_ports();
            capture_ports.connect_to_module(dj_fader);
        }

        public JackPorts create_input_ports(string name)
        {
            return create_ports(name, JackPortType.INPUT_PORT);
        }
    
        public JackPorts create_output_ports(string name)
        {
            return create_ports(name, JackPortType.OUTPUT_PORT);
        }
    
        JackPorts create_ports(string name, JackPortType type)
        {
            string port_name = name + (type == JackPortType.OUTPUT_PORT ? "_out_1" : "_in_1");
            unowned Jack.Port port1 = this.client.port_register(port_name, Jack.JACK_DEFAULT_AUDIO_TYPE, type == JackPortType.OUTPUT_PORT ? Jack.JackPort.IsOutput : Jack.JackPort.IsInput, 0);
            port_name = name + (type == JackPortType.OUTPUT_PORT ? "_out_2" : "_in_2");
            unowned Jack.Port port2 = this.client.port_register(port_name, Jack.JACK_DEFAULT_AUDIO_TYPE, type == JackPortType.OUTPUT_PORT ? Jack.JackPort.IsOutput : Jack.JackPort.IsInput, 0);
            return new JackPorts(this.client, port1, port2, type);
        }
    
        public void register_module(JackModule module)
        {
            this.modules += module;
        }
 
        JackPorts get_playback_ports()
        {
            unowned Jack.Port port1 = this.client.port_by_name("system:playback_1");
            unowned Jack.Port port2 = this.client.port_by_name("system:playback_2");

            return new JackPorts(this.client, port1, port2, JackPortType.INPUT_PORT);
        }

        JackPorts get_capture_ports()
        {
            unowned Jack.Port port1 = this.client.port_by_name("system:capture_1");
            unowned Jack.Port port2 = this.client.port_by_name("system:capture_2");

            return new JackPorts(this.client, port1, port2, JackPortType.OUTPUT_PORT);
        }
    }

}