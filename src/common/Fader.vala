namespace Common
{
    class Fader : VerticalBox
    {
        public Fader(string name, double gain)
        {
            var scale = new Gtk.Scale.with_range(Gtk.Orientation.VERTICAL, 0.0, 1.2, 0.05);
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
            this.pack_start(scale);

            var event_box = new Gtk.EventBox();
            this.pack_end(event_box, false);
            var label = new Gtk.Label(name);
            event_box.add(label);
        }
    }
}
