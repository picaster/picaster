class TrackButton : BaseMusicButton
{
    public TrackButton(int index)
    {
        base(@"TRK$index");

        var progress_bar = new Gtk.ProgressBar();
        progress_bar.set_show_text(false);
        progress_bar.set_fraction(0.0);
        this.box.pack_end(progress_bar);

        this.add_accelerator("clicked", PiCaster.App.accel_group, Gdk.Key.@0 + index, 0, Gtk.AccelFlags.VISIBLE);
    }
}
