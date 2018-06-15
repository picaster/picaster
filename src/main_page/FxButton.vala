class FxButton : BaseMusicButton
{
    public FxButton(int index)
    {
        base(@"FX$index");

        this.add_accelerator("accel", PiCaster.App.accel_group, Gdk.Key.@0 + index, Gdk.ModifierType.CONTROL_MASK, Gtk.AccelFlags.VISIBLE);
    }
}
