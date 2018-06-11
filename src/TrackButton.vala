class TrackButton : BaseMusicButton
{
    public TrackButton(int index)
    {
        var box = new Gtk.Box(Gtk.Orientation.VERTICAL, 4);
        this.add(box);

        var label = new Gtk.Label(@"TRK$index");
        box.pack_start(label);

        var progress_label = new Gtk.Label("00:00");
        box.pack_end(progress_label);

        var progress_bar = new Gtk.ProgressBar();
        progress_bar.set_show_text(false);
        progress_bar.set_fraction(0.33);
        box.pack_end(progress_bar);

        this.add_accelerator("clicked", PiCaster.accel_group, Gdk.Key.@0 + index, 0, Gtk.AccelFlags.VISIBLE);
    }
}
