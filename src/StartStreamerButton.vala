class StartStreamerButton : Gtk.ToggleButton
{
    public StartStreamerButton()
    {
        Object(
            label: "Stream"
        );

        this.add_accelerator("clicked", PiCaster.accel_group, Gdk.Key.s, 0, Gtk.AccelFlags.VISIBLE);
    }
}