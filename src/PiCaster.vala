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

namespace PiCaster { 
    
    public class App : Gtk.Application
    {
        public static Gtk.AccelGroup accel_group = new Gtk.AccelGroup();
        public static Gtk.ApplicationWindow main_window = null;
        public static Common.Bus bus = null;
        public static LibAudio.JackClient jack_client = null;

        public App()
        {
            Object(
                application_id: "ch.frenchguy.PiCaster",
                flags: ApplicationFlags.FLAGS_NONE
            );
            
            if (bus == null)
            {
                bus = new Common.Bus();
            }

            if (jack_client == null)
            {
                jack_client = new LibAudio.JackClient();
            }
        }
    
        protected override void activate()
        {
            PiCaster.App.main_window = new Gtk.ApplicationWindow(this);
            PiCaster.App.main_window.title = "PiCaster Desktop";
            PiCaster.App.main_window.add_accel_group(App.accel_group);
            PiCaster.App.main_window.set_default_size(600, 460);
            PiCaster.App.main_window.set_size_request(600, 460);
            PiCaster.App.main_window.set_resizable(false);
    
            var notebook = new Gtk.Notebook();
            PiCaster.App.main_window.add(notebook);
    
            notebook.append_page(new PiCaster.MainPage(), new Gtk.Label("Main"));
            notebook.append_page(new PiCaster.JackSettingsPage(), new Gtk.Label("Jack settings"));
            notebook.append_page(new PiCaster.RecorderSettingsPage(), new Gtk.Label("Recorder settings"));
            notebook.append_page(new Gtk.Box(Gtk.Orientation.HORIZONTAL, 4), new Gtk.Label("Stream settings"));
            notebook.append_page(new Gtk.Box(Gtk.Orientation.HORIZONTAL, 4), new Gtk.Label("Mumble settings"));

            PiCaster.App.main_window.show_all();
        }

        public static int main(string[] args) {
            var app = new App();
            var status = app.run(args);
            int width, height;
            PiCaster.App.main_window.get_size(out width, out height);
            PiCaster.App.jack_client.stop();
            return status;
        }
    }
}
