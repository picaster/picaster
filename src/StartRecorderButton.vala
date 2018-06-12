class StartRecorderButton : Gtk.ToggleButton
{
    public StartRecorderButton()
    {
        Object(
            label: "Record"
        );

        this.add_accelerator("clicked", PiCaster.accel_group, Gdk.Key.r, 0, Gtk.AccelFlags.VISIBLE);
    }
}