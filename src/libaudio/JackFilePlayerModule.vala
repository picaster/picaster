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
    public class JackFilePlayerModule : JackModule
    {
        public int index;
        private bool playing;
        AVFormat.Context fmt_ctx;
        int64 duration;

        public JackFilePlayerModule(string name, JackClient jack_client)
        {
            base(name, jack_client);
            this.playing = false;

            AVFormat.register_all();
            this.fmt_ctx = new AVFormat.Context();
        }

        public override void process(Jack.NFrames nframes)
        {
        }

        public bool is_playing()
        {
            return this.playing;
        }

        public void play(string filename)
        {
            stderr.printf("filename : %s\n", filename);
            if (AVFormat.open_input(ref this.fmt_ctx, filename, null, null) == 0)
            {
                stderr.printf("Error : AVFormat.open_input()\n");
                //exit(1);
            }

            if (this.fmt_ctx.find_stream_info(null) < 0)
            {
                stderr.printf("Error : AVFormat.find_stream()\n");
                //exit(1);
            }

            this.duration = this.fmt_ctx.duration;

            this.fmt_ctx.dump_format(0, filename, 0);

            // find audio stream in format context
            size_t stream = 0;
            for (; stream < fmt_ctx.nb_streams; stream++) {
                if (fmt_ctx.streams[stream].codec.codec_type == AVFormat.MediaType.AUDIO) {
                    break;
                }
            }

            if (stream == fmt_ctx.nb_streams) {
                stderr.printf("Error: no audio stream found\n");
                //exit(1);
            }

            unowned AVFormat.CodecContext codec_ctx = fmt_ctx.streams[stream].codec;
            if (codec_ctx.channel_layout == 0)
            {
                if (codec_ctx.channels == 1)
                {
                    codec_ctx.channel_layout = AVFormat.ChannelLayout.MONO;
                }
                else
                {
                    codec_ctx.channel_layout = AVFormat.ChannelLayout.STEREO;
                }
            }

            FFMpeg.Codec codec = FFMpeg.Codec.find_decoder(codec_ctx.codec_id);
            if (codec == null) {
                stderr.printf("Error: avcodec_find_decoder()\n");
                //exit(1);
            }
        
            this.playing = true;
        }

        public void stop()
        {
            this.playing = false;
        }
    }
}
