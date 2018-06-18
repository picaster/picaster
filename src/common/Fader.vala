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
    public delegate void FaderChanged(double value);

    class Fader : VerticalBox
    {
        private Gtk.Scale scale;

        public Fader(string name, double gain, FaderChanged fader_changed_callback)
        {
            this.name = name;

            scale = new Gtk.Scale.with_range(Gtk.Orientation.VERTICAL, 0.0, 1.2, 0.05);
            scale.set_inverted(true);
            scale.set_draw_value(false);
            scale.add_mark(1.11212562118, Gtk.PositionType.LEFT, "+3");
            scale.add_mark(1.00000000000, Gtk.PositionType.LEFT, "0");
            scale.add_mark(0.89917899646, Gtk.PositionType.LEFT, "-3");
            scale.add_mark(0.80852286768, Gtk.PositionType.LEFT, "-6");
            scale.add_mark(0.70170382867, Gtk.PositionType.LEFT, "-10");
            scale.add_mark(0.60899732444, Gtk.PositionType.LEFT, "-14");
            scale.add_mark(0.49238826317, Gtk.PositionType.LEFT, "-20");
            scale.add_mark(0.39810717055, Gtk.PositionType.LEFT, "-26");
            scale.add_mark(0.29986313485, Gtk.PositionType.LEFT, "-34");
            scale.add_mark(0.20309176209, Gtk.PositionType.LEFT, "-45");
            scale.add_mark(0.10000000000, Gtk.PositionType.LEFT, "-65");
            double value = GLib.Math.pow(10, gain / 65.0);
            scale.set_value(value);
            scale.value_changed.connect(() => {
                fader_changed_callback(scale.get_value());
            });
            this.pack_start(scale);

            var event_box = new Gtk.EventBox();
            this.pack_end(event_box, false);
            var label = new Gtk.Label(name);
            event_box.add(label);
            set_sensitive(false);

            PiCaster.App.bus.jack_started.connect(() => set_sensitive(true));
            PiCaster.App.bus.jack_stopped.connect(() => set_sensitive(false));
        }

        public void set_value(double value)
        {
            scale.set_value(value);
        }
    }
}
