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