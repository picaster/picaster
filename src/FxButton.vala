class FxButton : BaseMusicButton
{
    public FxButton(int index)
    {
        var box = new Gtk.Box(Gtk.Orientation.VERTICAL, 4);
        this.add(box);

        var label = new Gtk.Label(@"FX$index");
        box.pack_start(label);

        var progress_label = new Gtk.Label("00:00");
        box.pack_end(progress_label);

        this.add_accelerator("clicked", PiCaster.accel_group, Gdk.Key.@0 + index, Gdk.ModifierType.CONTROL_MASK, Gtk.AccelFlags.VISIBLE);
    }
}
