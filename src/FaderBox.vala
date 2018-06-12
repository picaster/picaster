class FaderBox : HorizontalBox
{
    public FaderBox()
    {
        this.pack_start(new Fader("DJ", 0.0));
        this.pack_start(new Gtk.Separator(Gtk.Orientation.VERTICAL));
        this.pack_start(new Fader("TR", 0.0));
        this.pack_start(new Gtk.Separator(Gtk.Orientation.VERTICAL));
        this.pack_start(new Fader("FX", 0.0));
        this.pack_start(new Gtk.Separator(Gtk.Orientation.VERTICAL));
        this.pack_start(new Fader("MA", -3.0));
    }
}

