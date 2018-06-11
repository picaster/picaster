class ControlBox : HorizontalBox
{
    public ControlBox()
    {
        this.set_homogeneous(true);
        this.pack_start(new StartJackButton());
        this.pack_start(new StartStreamerButton());
        this.pack_start(new StartRecorderButton());
    }
}