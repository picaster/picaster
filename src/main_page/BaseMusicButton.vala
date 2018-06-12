class BaseMusicButton : Gtk.ToggleButton
{
    protected Gtk.Box box;

    private bool loaded = false;

    public BaseMusicButton(string text)
    {
        this.box = new Gtk.Box(Gtk.Orientation.VERTICAL, 4);
        this.add(box);

        var label = new Gtk.Label(text);
        this.box.pack_start(label);

        var progress_label = new Gtk.Label("00:00");
        this.box.pack_end(progress_label);

        this.button_release_event.connect(this.button_released);
        this.button_press_event.connect(this.button_pressed);
        this.toggled.connect(this.toggledd);
    }

    private void toggledd()
    {
        stderr.printf("Toggled\n");
    }

    private bool button_pressed(Gdk.EventButton event)
    {
        if (!loaded)
        {
            stderr.printf("Not loaded\n");
            return true;
        }
        else
        {
            stderr.printf("Loaded\n");
            return false;
        }
    }

    private bool button_released(Gdk.EventButton event)
    {
        stderr.printf("button : %u\n", event.button);
        if (event.button == 3)
        {
            if (!loaded)
            {
                var dialog = new Gtk.FileChooserDialog("Select track", null, Gtk.FileChooserAction.OPEN, "Cancel", Gtk.ResponseType.CANCEL, "Open", Gtk.ResponseType.ACCEPT);
                Gtk.FileFilter filter = new Gtk.FileFilter();
                filter.add_mime_type("audio/*");
                dialog.set_filter(filter);
                if (dialog.run() == Gtk.ResponseType.ACCEPT) {
                }
                dialog.close();
            }
        }
        return false;
    }

}

