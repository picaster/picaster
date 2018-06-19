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
    public class JackRecorderModule : JackModule
    {
        private bool recording;
        private Jack.Ringbuffer rb;
        private RecorderThread recorder_thread;

        private size_t sample_size = sizeof(Jack.DefaultAudioSample);

        private Sndfile.File capture_sf;

        public JackRecorderModule(string name, JackClient jack_client)
        {
            base(name, jack_client);
            input_ports = jack_client.create_input_ports(name);
            recording = false;
            rb = Jack.Ringbuffer.create(2 * sample_size * 8192);
            PiCaster.App.bus.start_recorder.connect(() =>
            {
                this.start_recording("/tmp/picaster.flac");
            });
            PiCaster.App.bus.stop_recorder.connect(() =>
            {
                this.stop_recording();
            });
        }

        private void start_recording(string filepath)
        {
            Sndfile.Info sf_info = Sndfile.Info();
            sf_info.samplerate = this.jack_client.get_client().get_samplerate();
            sf_info.format = Sndfile.Format.FLAC | Sndfile.Format.PCM_24;
            sf_info.channels = 2;
            if ((capture_sf = new Sndfile.File(filepath, Sndfile.Mode.WRITE, ref sf_info)) == null)
            {
                stderr.printf("Error opening output file\n");
                exit(1);
            }
            this.recorder_thread = new RecorderThread();
            new Thread<int>("recorder_thread", recorder_thread.run);
            this.recording = true;
        }

        private void stop_recording()
        {
            this.recorder_thread.stop = true;
            this.recording = false;
            capture_sf.write_sync();
            capture_sf.close();
        }

        public override void process(Jack.NFrames nframes)
        {
            if (this.recording)
            {
                var input_buffers = this.get_input_ports_buffers(nframes);
                for (Jack.NFrames frame = 0; frame < nframes; frame++)
                {
                    capture_sf.writef_float({input_buffers[0][frame], input_buffers[1][frame]}, 1);
                }
            }
        }
    }

    protected class RecorderThread : Object
    {
        public bool stop;
        private int count = 0;
        protected RecorderThread()
        {
            this.stop = false;
        }

        public int run()
        {
            while (!stop)
            {
                stdout.printf ("%s: %i\n", "recorder_thread", this.count);
                this.count++;
                Thread.usleep (Random.int_range (0, 200000));
            }
            return 0;
        }
    }
}