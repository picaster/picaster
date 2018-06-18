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
    public enum JackPortType
    {
        INPUT_PORT,
        OUTPUT_PORT
    }

    public class JackPorts
    {
        private unowned Jack.Client client;
        private unowned Jack.Port port1;
        private unowned Jack.Port port2;
        private JackPortType type;

        public JackPorts(Jack.Client client, Jack.Port port1, Jack.Port port2, JackPortType type)
        {
            this.client = client;
            this.port1 = port1;
            this.port2 = port2;
            this.type = type;
            // buffers
        }

        public void connect_to_ports(JackPorts dest)
        {
            if (this.type == dest.type)
            {
                return;
            }
    
            this.client.connect(Jack.port_name(port1), Jack.port_name(dest.port1));
            this.client.connect(Jack.port_name(port2), Jack.port_name(dest.port2));
        }
    
        public void connect_to_module(JackModule module)
        {
            this.connect_to_ports(module.get_input_ports());
        }

        public float*[] get_buffers(Jack.NFrames nframes)
        {
            var buffer1 = this.port1.get_buffer(nframes);
            var buffer2 = this.port2.get_buffer(nframes);
            return {buffer1, buffer2};
        }
    }    

}