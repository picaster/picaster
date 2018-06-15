class StartRecorderButton : Gtk.ToggleButton
{
    public StartRecorderButton()
    {
        Object(
            label: "Record"
        );

        this.add_accelerator("clicked", PiCaster.App.accel_group, Gdk.Key.r, 0, Gtk.AccelFlags.VISIBLE);

        set_sensitive(false);
        PiCaster.App.bus.jack_started.connect(() => set_sensitive(true));
        PiCaster.App.bus.jack_stopped.connect(() => set_sensitive(false));

        toggled.connect(() => {
            if (get_active()) PiCaster.App.bus.lock_jack();
            else PiCaster.App.bus.unlock_jack();
        });
    }
}