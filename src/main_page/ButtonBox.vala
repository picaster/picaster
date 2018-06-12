class ButtonBox : Common.VerticalBox
{
    public ButtonBox()
    {
        for (var row = 0; row < 4; row++)
        {
            var row_box = new Gtk.Box(Gtk.Orientation.HORIZONTAL, 4);
            this.pack_start(row_box);
            for (var col = 0; col < 2; col++)
            {
                var index = 1 + row * 2 + col;
                var button = new TrackButton(index);
                row_box.pack_start(button);
            }
        }

        var separator = new Gtk.Separator(Gtk.Orientation.HORIZONTAL);
        this.pack_start(separator);

        for (var row = 0; row < 2; row++)
        {
            var row_box = new Gtk.Box(Gtk.Orientation.HORIZONTAL, 4);
            this.pack_start(row_box);
            for (var col = 0; col < 3; col++)
            {
                var index = 1 + 3 * row + col;
                var button = new FxButton(index);
                row_box.pack_start(button);
            }
        }

        separator = new Gtk.Separator(Gtk.Orientation.HORIZONTAL);
        this.pack_start(separator);

        var mic_button = new MicButton();
        this.pack_end(mic_button);
        
    }
}

