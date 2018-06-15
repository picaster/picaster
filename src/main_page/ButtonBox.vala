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

class ButtonBox : Common.VerticalBox
{
    public ButtonBox()
    {
        for (var row = 0; row < 4; row++)
        {
            var row_box = new Gtk.Box(Gtk.Orientation.HORIZONTAL, 4);
            row_box.set_homogeneous(true);
            this.pack_start(row_box);
            for (var col = 0; col < 2; col++)
            {
                var index = 1 + row * 2 + col;
                var button = new TrackButton(index);
                row_box.pack_start(button);
            }
        }

        var separator = new Gtk.Separator(Gtk.Orientation.HORIZONTAL);
        this.pack_start(separator);

        for (var row = 0; row < 2; row++)
        {
            var row_box = new Gtk.Box(Gtk.Orientation.HORIZONTAL, 4);
            row_box.set_homogeneous(true);
            this.pack_start(row_box);
            for (var col = 0; col < 3; col++)
            {
                var index = 1 + 3 * row + col;
                var button = new FxButton(index);
                row_box.pack_start(button);
            }
        }

        separator = new Gtk.Separator(Gtk.Orientation.HORIZONTAL);
        this.pack_start(separator);

        var mic_button = new MicButton();
        this.pack_end(mic_button);
        
    }
}

