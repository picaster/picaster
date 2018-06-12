class FaderBox : Common.HorizontalBox
{
    public FaderBox()
    {
        this.pack_start(new Common.Fader("DJ", 0.0));
        this.pack_start(new Gtk.Separator(Gtk.Orientation.VERTICAL));
        this.pack_start(new Common.Fader("TR", 0.0));
        this.pack_start(new Gtk.Separator(Gtk.Orientation.VERTICAL));
        this.pack_start(new Common.Fader("FX", 0.0));
        this.pack_start(new Gtk.Separator(Gtk.Orientation.VERTICAL));
        this.pack_start(new Common.Fader("MA", -3.0));
    }
}
