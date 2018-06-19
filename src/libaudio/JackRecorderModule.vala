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
    /*
    public class RingBuf
    {
        private const int size = 2 * 8192;
        private float[] buffer;
        private int head;
        private int tail;
        public int length { get; private set; }

        public RingBuf()
        {
            buffer = new float[size];
            head = 0;
            tail = 0;
        }

        public void write(float f)
        {
            buffer[tail] = f;
            tail += 1;
            length += 1;
            if (tail >= size)
            {
                tail = 0;
            }
        }

        public float read()
        {
            float f = buffer[head];
            head += 1;
            length -= 1;
            if (head >= size)
            {
                head = 0;
            }
            return f;
        }
    }
    */

    public class JackRecorderModule : JackModule
    {
        private bool recording;
        private Sndfile.File capture_sf;
        /*
        private RingBuf ring_buf;
        private GLib.Mutex disk_thread_lock = GLib.Mutex();
        private GLib.Cond data_ready = GLib.Cond();
        private Thread<int> recorder_thread;
        */

        public JackRecorderModule(string name, JackClient jack_client)
        {
            base(name, jack_client);
            input_ports = jack_client.create_input_ports(name);
            recording = false;
            /*
            ring_buf = new RingBuf();
            */
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
            this.recording = true;
            /*
            this.recorder_thread = new Thread<int>("recorder_thread", this.run);
            */
        }

        /*
        private int run()
        {
            disk_thread_lock.lock();
            while (recording)
            {
                data_ready.wait(disk_thread_lock);
                while (this.ring_buf.length > 2)
                {
                    capture_sf.writef_float({this.ring_buf.read(), this.ring_buf.read()}, 1);
                }
            }
            stderr.printf("run: not recording, unlocking disk_thread_lock\n");
            disk_thread_lock.unlock();
            stderr.printf("run: unlocked disk_thread_lock, returning\n");
            return 0;
        }
        */

        private void stop_recording()
        {
            this.recording = false;
            /*
            stderr.printf("stop_recording: locking disk_thread_lock\n");
            disk_thread_lock.@lock();
            stderr.printf("stop_recording: signaling data_ready\n");
            data_ready.@signal();
            stderr.printf("stop_recording: unlocking disk_thread_lock\n");
            disk_thread_lock.unlock();
            stderr.printf("stop_recording: waiting for recorder_thread to finish\n");
            this.recorder_thread.join();
            stderr.printf("stop_recording: recorder_thread has finished\n");
            */
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
                    /*
                    for (int chn = 0; chn < 2; chn++)
                    {
                        this.ring_buf.write(input_buffers[chn][frame]);
                    }
                    */
                }
                /*
                if (disk_thread_lock.trylock())
                {
                    data_ready.@signal();
                    disk_thread_lock.unlock();
                }
                */
            }
        }
    }
}