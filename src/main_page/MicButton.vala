class MicButton : Gtk.ToggleButton
{
    public MicButton()
    {
        Object(
            label: "Mic"
        );

        this.add_accelerator("clicked", PiCaster.App.accel_group, Gdk.Key.m, 0, Gtk.AccelFlags.VISIBLE);
    }
}
