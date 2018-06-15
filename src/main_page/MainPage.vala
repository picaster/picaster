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

namespace PiCaster
{
    class MainPage : Common.HorizontalBox
    {
        public MainPage()
        {
            this.set_margin_top(4);
            this.set_margin_bottom(4);
            this.set_margin_start(4);
            this.set_margin_end(4);
    
            this.pack_start(new ButtonBox());
            this.pack_start(new Gtk.Separator(Gtk.Orientation.VERTICAL));
    
            var box = new Gtk.Box(Gtk.Orientation.VERTICAL, 4);
            this.pack_start(box);
    
            box.pack_start(new FaderBox());
            box.pack_start(new Gtk.Separator(Gtk.Orientation.HORIZONTAL), false);
            box.pack_end(new ControlBox(), false);
        }
    }
}
