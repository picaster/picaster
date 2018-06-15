/**
    PiCaster is a podcast recording and streaming application,
    easy to use for beginners, but with enough functionality
    to suit the needs of intermediate or experienced podcasters.
    Copyright (C) 2018  Yannick Mauray
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

extern void exit(int exit_code);

public abstract class BaseMusicButton : Gtk.Box
{
    private new string name;
    private Gtk.Label name_label;
    private Gtk.Label progress_label;
    private string file = null;
    protected Gtk.ToggleButton button;

    private bool jack_started;

    public BaseMusicButton(string name)
    {
        Object(
            orientation: Gtk.Orientation.VERTICAL, 
            spacing: 4
        );

        this.name = name;
        this.file = Common.Settings.get_string("track", this.name, "");
        var info = Common.Settings.get_string("track", @"$name.info", this.name);
        var duration = Common.Settings.get_string("track", @"$name.duration", "00:00");

        if (this.file == "")
        {
            this.file = null;
        }

        button = new Gtk.ToggleButton();
        this.pack_start(button);
        var box = new Gtk.Box(Gtk.Orientation.VERTICAL, 4);
        button.add(box);

        this.name_label = new Gtk.Label(info);
        this.name_label.set_ellipsize(Pango.EllipsizeMode.START);
        this.name_label.set_max_width_chars(0);
        box.pack_start(this.name_label);

        this.progress_label = new Gtk.Label(duration);
        box.pack_end(this.progress_label);

        button.button_release_event.connect(this.button_released);
        button.button_press_event.connect(this.button_pressed);
        button.toggled.connect(this.button_toggled);

        this.jack_started = false;
        PiCaster.App.bus.jack_started.connect(() => jack_started = true);
        PiCaster.App.bus.jack_stopped.connect(() => jack_started = false);

        accel.connect(() => {
            if (file != null && jack_started)
            {
                this.button.set_active(!this.button.get_active());
            }
        });
    }

    private void button_toggled()
    {
        if (button.get_active())
        {
            PiCaster.App.bus.lock_jack();
            play();
        }
        else
        {
            PiCaster.App.bus.unlock_jack();
            stop();
        }
    }

    private bool button_pressed(Gdk.EventButton event)
    {
        if ((file == null) || (!jack_started))
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

    [Signal (action=true)]
    public signal void accel();

    protected abstract void play();

    protected abstract void stop();
}
