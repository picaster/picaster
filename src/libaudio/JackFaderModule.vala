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
    public class JackFaderModule : JackModule
    {
        private double slider_value;
        private double db_value;
        private double amplification;

        public JackFaderModule(string name, JackClient jack_client)
        {
            base(name, jack_client);
            input_ports = jack_client.create_input_ports(name);
            output_ports = jack_client.create_output_ports(name);
            this.amplification = 1.0f;
        }

        public override void process(Jack.NFrames nframes)
        {
            var input_buffers = this.get_input_ports_buffers(nframes);
            var output_buffers = this.get_output_ports_buffers(nframes);
            for (Jack.NFrames frame = 0; frame < nframes; frame++)
            {
                output_buffers[0][frame] = (float)(this.amplification * input_buffers[0][frame]);
                output_buffers[1][frame] = (float)(this.amplification * input_buffers[1][frame]);
            }
        }

        public void set_slider_value(double slider_value)
        {
            this.slider_value = slider_value;
            this.db_value = 65 * Math.log10(slider_value);
            this.amplification = Math.pow(10, db_value / 20.0f);
        }
    }
}
