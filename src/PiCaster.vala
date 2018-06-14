namespace PiCaster { 
    
    class App : Gtk.Application
    {
        public static Gtk.AccelGroup accel_group = new Gtk.AccelGroup();
        public static Gtk.ApplicationWindow main_window = null;

        public App()
        {
            Object(
                application_id: "ch.frenchguy.PiCaster",
                flags: ApplicationFlags.FLAGS_NONE
            );
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
            notebook.append_page(new Gtk.Box(Gtk.Orientation.HORIZONTAL, 4), new Gtk.Label("Jack settings"));
            notebook.append_page(new Gtk.Box(Gtk.Orientation.HORIZONTAL, 4), new Gtk.Label("Stream settings"));
            notebook.append_page(new Gtk.Box(Gtk.Orientation.HORIZONTAL, 4), new Gtk.Label("Mumble settings"));

            PiCaster.App.main_window.show_all();
        }
    
        public static int main(string[] args) {
            var app = new App();
            var status = app.run(args);
            int width, height;
            PiCaster.App.main_window.get_size(out width, out height);
            stderr.printf("%d, %d\n", width, height);
            return status;
        }
    }
}
