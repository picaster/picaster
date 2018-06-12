class BaseMusicButton : Gtk.ToggleButton
{
    protected Gtk.Box box;
    
    private Gtk.Label label;
    private Gtk.Label progress_label;
    private string file = null;

    public BaseMusicButton(string text)
    {
        this.box = new Gtk.Box(Gtk.Orientation.VERTICAL, 4);
        this.add(box);

        this.label = new Gtk.Label(text);
        this.box.pack_start(this.label);

        this.progress_label = new Gtk.Label("00:00");
        this.box.pack_end(this.progress_label);

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
        if (file == null)
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
        if (event.button == 3)
        {
            if (file == null)
            {
                var dialog = new Common.FileChooser();
                if (dialog.run() == Gtk.ResponseType.ACCEPT) {
                    this.file = dialog.get_selection();
                    var taglib_file = new TagLib.File(this.file);
                    var audioproperties = taglib_file.audioproperties;
                    int length = audioproperties.length;
                    int seconds = length % 60;
                    int minutes = (length - seconds) / 60;
                    var ssec = seconds.to_string("%02d");
                    var smin = minutes.to_string("%02d");
                    string duration = @"$smin:$ssec";
                    this.progress_label.set_text(duration);

                    /*var tag = taglib_file.tag;
                    string title = tag.title;
                    string artist = tag.artist;
                    var track_info = @"$artist - $title";*/
                }
                dialog.close();
            }
        }
        return false;
    }
}