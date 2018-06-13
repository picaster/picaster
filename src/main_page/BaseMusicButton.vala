extern void exit(int exit_code);

class BaseMusicButton : Gtk.ToggleButton
{
    protected Gtk.Box box;
    
    private Gtk.Label name_label;
    private Gtk.Label progress_label;
    private string file = null;

    public BaseMusicButton(string text)
    {
        this.box = new Gtk.Box(Gtk.Orientation.VERTICAL, 4);
        this.add(box);

        this.name_label = new Gtk.Label(text);
        this.name_label.ellipsize = Pango.EllipsizeMode.START;
        this.box.pack_start(this.name_label);

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
            return true;
        }
        else
        {
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
                    this.file = dialog.get_filename();
                    var taglib_file = new TagLib.File(this.file);
                    if (taglib_file == null)
                    {
                        stderr.printf("taglib_file is null\n");
                        exit(-1);
                    }
                    if (!taglib_file.is_valid())
                    {
                        stderr.printf("taglib_file is not valid\n");
                        exit(-1);
                    }
                    int length = taglib_file.audioproperties.length;
                    int seconds = length % 60;
                    int minutes = (length - seconds) / 60;
                    var ssec = seconds.to_string("%02d");
                    var smin = minutes.to_string("%02d");
                    string duration = @"$smin:$ssec";
                    this.progress_label.set_text(duration);

                    string title = taglib_file.tag.title;
                    string artist = taglib_file.tag.artist;
                    if ((title == "") || (artist == ""))
                    {
                        this.name_label.set_text(this.file);
                    }
                    else
                    {
                        var track_info = @"$artist - $title";
                        this.name_label.set_text(track_info);
                    }
                }
                dialog.close();
            }
            else
            {
            }
        }
        return false;
    }
}