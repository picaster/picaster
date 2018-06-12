namespace PiCaster
{
    class MainPage : Common.HorizontalBox
    {
        public MainPage()
        {
            this.set_margin_top(4);
            this.set_margin_bottom(4);
            this.set_margin_start(4);
            this.set_margin_end(4);
    
            this.pack_start(new ButtonBox());
            this.pack_start(new Gtk.Separator(Gtk.Orientation.VERTICAL));
    
            var box = new Gtk.Box(Gtk.Orientation.VERTICAL, 4);
            this.pack_start(box);
    
            box.pack_start(new FaderBox());
            box.pack_start(new Gtk.Separator(Gtk.Orientation.HORIZONTAL), false);
            box.pack_end(new ControlBox(), false);
        }
    }
}
