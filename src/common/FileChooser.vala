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
        }

        public new int run()
        {
            if (Common.FileChooser.current_folder != null)
            {
                this.set_current_folder(Common.FileChooser.current_folder);
            }
            return base.run();
        }

        public string get_selection()
        {
            SList<string> uris = get_uris();
            return uris.nth_data(0);
        }

        public new void close()
        {
            Common.FileChooser.current_folder = this.get_current_folder();
            base.close();
        }
    }
}