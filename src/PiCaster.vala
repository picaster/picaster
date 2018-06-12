namespace PiCaster { 
    
    class App : Gtk.Application
    {
        public static Gtk.AccelGroup accel_group = new Gtk.AccelGroup();
    
        public App()
        {
            Object(
                application_id: "ch.frenchguy.PiCaster",
                flags: ApplicationFlags.FLAGS_NONE
            );
        }
    
        protected override void activate()
        {
            var main_window = new Gtk.ApplicationWindow(this);
            main_window.title = "PiCaster Desktop";
            main_window.add_accel_group(App.accel_group);
            /*
            main_window.default_height = 300;
            main_window.default_width = 300;
            */
    
            var notebook = new Gtk.Notebook();
            main_window.add(notebook);
    
            notebook.append_page(new PiCaster.MainPage(), new Gtk.Label("Main"));
            notebook.append_page(new Gtk.Box(Gtk.Orientation.HORIZONTAL, 4), new Gtk.Label("Jack settings"));
            notebook.append_page(new Gtk.Box(Gtk.Orientation.HORIZONTAL, 4), new Gtk.Label("Stream settings"));
            notebook.append_page(new Gtk.Box(Gtk.Orientation.HORIZONTAL, 4), new Gtk.Label("Mumble settings"));
    
            main_window.show_all ();
        }
    
        public static int main(string[] args) {
            var app = new App();
            return app.run(args);
        }
    }
}
