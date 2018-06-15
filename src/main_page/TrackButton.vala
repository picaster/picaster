class TrackButton : BaseMusicButton
{
    public TrackButton(int index)
    {
        base(@"TRK$index");

        var progress_bar = new Gtk.ProgressBar();
        progress_bar.set_show_text(false);
        progress_bar.set_fraction(0.0);
        this.box.pack_end(progress_bar);

        this.add_accelerator("accel", PiCaster.App.accel_group, Gdk.Key.@0 + index, 0, Gtk.AccelFlags.VISIBLE);

        set_sensitive(false);
        PiCaster.App.bus.jack_started.connect(() => set_sensitive(true));
        PiCaster.App.bus.jack_stopped.connect(() => set_sensitive(false));

        toggled.connect(() => {
            if (get_active()) PiCaster.App.bus.lock_jack();
            else PiCaster.App.bus.unlock_jack();
        });

        accel.connect(() => {
            stderr.printf("accel!\n");
        });
    }

    [Signal (action=true)]
    public signal void accel();
}
