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

namespace Common
{
    class FileChooser : Gtk.FileChooserDialog
    {
        private static string current_folder = null;

        public FileChooser()
        {
            Object(
                title: "Select file",
                parent: null,
                action: Gtk.FileChooserAction.OPEN
            );
            this.add_button("Cancel", Gtk.ResponseType.CANCEL);
            this.add_button("Open", Gtk.ResponseType.ACCEPT);
            Gtk.FileFilter filter = new Gtk.FileFilter();
            filter.add_mime_type("audio/*");
            this.set_filter(filter);
            current_folder = Common.Settings.get_string("track", "current_folder", GLib.Environment.get_home_dir());
        }

        public new int run()
        {
            if (Common.FileChooser.current_folder != null)
            {
                this.set_current_folder(Common.FileChooser.current_folder);
            }
            return base.run();
        }

        public new void close()
        {
            Common.FileChooser.current_folder = this.get_current_folder();
            Common.Settings.save_string("track", "current_folder", Common.FileChooser.current_folder);
            base.close();
        }
    }
}