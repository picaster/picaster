class MicButton : Gtk.ToggleButton
{
    public MicButton()
    {
        Object(
            label: "Mic"
        );

        this.add_accelerator("clicked", PiCaster.accel_group, Gdk.Key.m, 0, Gtk.AccelFlags.VISIBLE);
    }
}
