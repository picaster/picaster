class StartJackButton : Gtk.ToggleButton
{
    public StartJackButton()
    {
        Object(
            label: "Jack"
        );

        this.add_accelerator("clicked", PiCaster.App.accel_group, Gdk.Key.j, 0, Gtk.AccelFlags.VISIBLE);
    }
}