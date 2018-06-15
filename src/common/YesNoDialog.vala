namespace Common
{ 
    public class YesNoDialog : Gtk.Dialog
    {
        private Gtk.Label question_label;

        public YesNoDialog(string title, string question)
        {
            this.title = title;
            this.border_width = 4;
            set_default_size(350, 100);
            create_widgets(question);
            connect_signals();
        }

        private void create_widgets(string question)
        {
            question_label = new Gtk.Label("plop");

            Gtk.Box content = get_content_area() as Gtk.Box;
            content.pack_start(question_label, false, true, 0);
            content.spacing = 10;

            add_button("Yes", Gtk.ResponseType.YES);
            add_button("No", Gtk.ResponseType.NO);
        }

        private void connect_signals()
        {
            this.response.connect(on_response);
        }

        private void on_response(Gtk.Dialog source, int response_id)
        {

        }
    }
}
