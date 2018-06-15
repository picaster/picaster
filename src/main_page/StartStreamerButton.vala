class StartStreamerButton : Gtk.ToggleButton
{
    public StartStreamerButton()
    {
        Object(
            label: "Stream"
        );

        this.add_accelerator("clicked", PiCaster.App.accel_group, Gdk.Key.s, 0, Gtk.AccelFlags.VISIBLE);

        set_sensitive(false);
        PiCaster.App.bus.jack_started.connect(jack_started);
        PiCaster.App.bus.jack_stopped.connect(jack_stopped);

        toggled.connect(() => {
            if (get_active()) PiCaster.App.bus.lock_jack();
            else PiCaster.App.bus.unlock_jack();
        });
    }

    public void jack_started()
    {
        set_sensitive(true);
    }

    public void jack_stopped()
    {
        set_sensitive(false);
    }
}