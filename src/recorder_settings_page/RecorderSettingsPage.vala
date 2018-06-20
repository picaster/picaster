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

namespace PiCaster
{ 
    public class RecorderSettingsPage : Common.SettingsPage
    {
        public RecorderSettingsPage()
        {
            var output_directory_file_chooser = new Gtk.FileChooserButton("Select output folder", Gtk.FileChooserAction.SELECT_FOLDER);
            output_directory_file_chooser.set_filename(Common.Settings.get_string("recorder", "output_directory", Environment.get_home_dir()));
            output_directory_file_chooser.select_multiple = false;
            output_directory_file_chooser.file_set.connect(() => {
                Common.Settings.save_string("recorder", "output_directory", output_directory_file_chooser.get_filename());
            });
            add_field("Output directory", output_directory_file_chooser);
        }
    }
}
