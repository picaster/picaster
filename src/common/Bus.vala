namespace Common
{ 
    public class Bus : GLib.Object
    { 
        public Bus()
        {
            
        }

        public signal void jack_started();
        public signal void jack_stopped();
        public signal void lock_jack();
        public signal void unlock_jack();
    }
}