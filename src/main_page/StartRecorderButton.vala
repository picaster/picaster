class StartRecorderButton : Gtk.ToggleButton
{
    public StartRecorderButton()
    {
        Object(
            label: "Record"
        );

        this.add_accelerator("clicked", PiCaster.App.accel_group, Gdk.Key.r, 0, Gtk.AccelFlags.VISIBLE);
    }
}