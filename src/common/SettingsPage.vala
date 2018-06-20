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
    protected class SettingsPage : Gtk.Grid
    {
        private int row;

        protected SettingsPage()
        {
            this.column_spacing = 4;
            this.row_spacing = 4;
            this.margin_top = 4;
            this.margin_bottom = 4;
            this.margin_start = 4;
            this.margin_end = 4;
            this.row = 0;
        }

        protected void add_field(string label_text, Gtk.Widget field)
        {
            var label = new Gtk.Label(label_text + " :");
            label.justify = Gtk.Justification.RIGHT;
            this.attach(label, 0, this.row);
            this.attach(field, 1, this.row);
            this.row += 1;
        }
    }
}
