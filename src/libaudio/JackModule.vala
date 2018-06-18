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
    public abstract class JackModule
    {
        private string name;
        protected JackClient jack_client;

        public JackPorts input_ports;
        public JackPorts output_ports;

        public JackModule(string name, JackClient jack_client)
        {
            this.name = name;
            this.jack_client = jack_client;
        }

        public JackPorts get_input_ports()
        {
            return input_ports;
        }
        
        public JackPorts get_output_ports()
        {
            return output_ports;
        }

        public void connect_to_ports(JackPorts input_ports)
        {
            this.output_ports.connect_to_ports(input_ports);
        }

        public void connect_to_module(JackModule module)
        {
            this.output_ports.connect_to_ports(module.input_ports);
        }

        public float*[] get_input_ports_buffers(Jack.NFrames nframes)
        {
            return input_ports.get_buffers(nframes);
        }

        public float*[] get_output_ports_buffers(Jack.NFrames nframes)
        {
            return output_ports.get_buffers(nframes);
        }

        public abstract void process(Jack.NFrames nframes);
    }
}