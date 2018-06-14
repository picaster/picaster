extern void exit(int exit_code);

class BaseMusicButton : Gtk.ToggleButton
{
    protected Gtk.Box box;
    
    private new string name;
    private Gtk.Label name_label;
    private Gtk.Label progress_label;
    private string file = null;

    public BaseMusicButton(string name)
    {
        this.box = new Gtk.Box(Gtk.Orientation.VERTICAL, 4);
        this.add(box);

        this.name = name;
        this.file = Common.Settings.get_string("track", this.name, "");
        var info = Common.Settings.get_string("track", @"$name.info", this.name);
        var duration = Common.Settings.get_string("track", @"$name.duration", "00:00");

        stderr.printf("%s : %s\n", this.name, this.file);

        if (this.file == "")
        {
            this.file = null;
        }

        this.name_label = new Gtk.Label(info);
        this.name_label.set_ellipsize(Pango.EllipsizeMode.START);
        this.name_label.set_max_width_chars(0);
        this.box.pack_start(this.name_label);

        this.progress_label = new Gtk.Label(duration);
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
                    Common.Settings.save_string("track", @"$name", file);
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
                    Common.Settings.save_string("track", @"$name.duration", duration);

                    string title = taglib_file.tag.title;
                    string artist = taglib_file.tag.artist;
                    if ((title == "") || (artist == ""))
                    {
                        this.name_label.set_text(this.file);
                        Common.Settings.save_string("track", @"$name.info", this.file);
                    }
                    else
                    {
                        var track_info = @"$artist - $title";
                        this.name_label.set_text(track_info);
                        Common.Settings.save_string("track", @"$name.info", track_info);
                    }
                }
                dialog.close();
            }
            else
            {
                var msg = new Gtk.MessageDialog(
                    PiCaster.App.main_window, Gtk.DialogFlags.MODAL, 
                    Gtk.MessageType.QUESTION, Gtk.ButtonsType.YES_NO,
                    "Are you sure you want to clear this track ?"
                );

                msg.response.connect((response_id) => {
                    switch (response_id)
                    {
                        case Gtk.ResponseType.YES:
                            this.name_label.set_text(this.name);
                            Common.Settings.delete_key("track", @"$name.info");
                            this.progress_label.set_text("00:00");
                            Common.Settings.delete_key("track", @"$name.duration");
                            this.file = null;
                            Common.Settings.delete_key("track", this.name);
                            break;
                        case Gtk.ResponseType.NO:
                            break;
                    }
                    msg.destroy();
                });
                msg.show();
            }
        }
        return false;
    }
}