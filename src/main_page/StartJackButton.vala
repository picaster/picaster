public class StartJackButton : Gtk.ToggleButton
{
    private int lock_count;

    public StartJackButton()
    {
        Object(
            label: "Jack"
        );

        this.add_accelerator("clicked", PiCaster.App.accel_group, Gdk.Key.j, 0, Gtk.AccelFlags.VISIBLE);
        this.toggled.connect(this.button_toggled);

        lock_count = 0;
        
        PiCaster.App.bus.lock_jack.connect(() => {
            lock_count++;
            set_sensitive(false);
        });

        PiCaster.App.bus.unlock_jack.connect(() => {
            if (lock_count > 0) lock_count--;
            if (lock_count == 0) set_sensitive(true);
        });
    }

    private void button_toggled()
    {
        if (this.get_active())
        {
            PiCaster.App.bus.jack_started();
        }
        else
        {
            PiCaster.App.bus.jack_stopped();
        }
    }
}