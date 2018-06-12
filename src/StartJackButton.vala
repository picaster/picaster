class StartJackButton : Gtk.ToggleButton
{
    public StartJackButton()
    {
        Object(
            label: "Jack"
        );

        this.add_accelerator("clicked", PiCaster.accel_group, Gdk.Key.j, 0, Gtk.AccelFlags.VISIBLE);
    }
}