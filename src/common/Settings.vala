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
    class Settings : GLib.Object
    {
        private static KeyFile file = null;
        private const string ini_file = "picaster.ini";
        private static string full_path;

        public static void save_string(string section, string key, string value)
        {
            unowned KeyFile file = Settings.get_file();
            file.set_string(section, key, value);
            try
            {
                file.save_to_file(Settings.full_path);
            }
            catch (GLib.FileError e)
            {
                ;
            }
        }

        public static string get_string(string section, string key, string default_value)
        {
            unowned KeyFile file = Settings.get_file();
            try
            {
                var value = file.get_string(section, key);
                return value;
            }
            catch (GLib.KeyFileError e)
            {
                return default_value;
            }
        }

        public static void delete_key(string section, string key)
        {
            unowned KeyFile file = Settings.get_file();
            try
            {
                file.remove_key(section, key);
                file.save_to_file(Settings.full_path);
            }
            catch (GLib.KeyFileError e)
            {
                ;
            }
            catch (GLib.FileError e)
            {
                ;
            }
        }

        private static unowned KeyFile get_file()
        {
            if (Settings.file == null)
            {
                Settings.file = new KeyFile();
                try
                {
                    Settings.file.load_from_data_dirs(Settings.ini_file, out Settings.full_path, KeyFileFlags.NONE);
                }
                catch (GLib.KeyFileError e1)
                {
                    ; 
                }
                catch (GLib.FileError e2)
                {
                    ;
                }
            }

            return Settings.file;
        }
    }
}