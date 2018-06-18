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

namespace Common
{ 
    public class Bus : GLib.Object
    { 
        public Bus()
        {
            
        }

        public signal void jack_started();
        public signal void jack_stopped();

        public signal void lock_jack();
        public signal void unlock_jack();

        public signal void lock_fx_button();
        public signal void unlock_fx_button();
    
        public signal void lock_track_button();
        public signal void unlock_track_button();

        public signal void start_recorder();
        public signal void stop_recorder();
    
    }
}