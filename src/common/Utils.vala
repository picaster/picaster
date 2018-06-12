namespace Common { 
    class VerticalBox : Gtk.Box
    {
        public VerticalBox()
        {
            Object(
                orientation: Gtk.Orientation.VERTICAL,
                spacing: 4
            );
        }
    }
    
    class HorizontalBox : Gtk.Box
    {
        public HorizontalBox()
        {
            Object(
                orientation: Gtk.Orientation.HORIZONTAL,
                spacing: 4
            );
        }
    }
}
