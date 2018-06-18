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
    public class JackSineWaveGeneratorModule : JackModule
    {
        private int index;

        public JackSineWaveGeneratorModule(string name, JackClient jack_client)
        {
            base(name, jack_client);
            output_ports = jack_client.create_output_ports(name);
            this.index = 0;
        }

        public override void process(Jack.NFrames nframes)
        {
            var output_buffers = this.get_output_ports_buffers(nframes);
            for (Jack.NFrames frame = 0; frame < nframes; frame++)
            {
                var alpha = index * 2 * Math.PI / 10000;
                var gamma = index * 2 * Math.PI / 5000;
                var cosa = (float)Math.cos(alpha);
                var cosg = (float)Math.cos(gamma);
                output_buffers[0][frame] = cosa;
                output_buffers[1][frame] = cosg;
                index += 1;
                if (index == 10000) index = 0;
            }
        }
    }
}
