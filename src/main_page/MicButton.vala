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

class MicButton : Gtk.ToggleButton
{
    public MicButton()
    {
        Object(
            label: "Mic"
        );

        this.add_accelerator("clicked", PiCaster.App.accel_group, Gdk.Key.m, 0, Gtk.AccelFlags.VISIBLE);

        set_sensitive(false);
        PiCaster.App.bus.jack_started.connect(() => set_sensitive(true));
        PiCaster.App.bus.jack_stopped.connect(() => set_sensitive(false));

        toggled.connect(() => {
            if (get_active())
            {
                PiCaster.App.bus.lock_jack();
                PiCaster.App.bus.mic_button_active(true);
            }
            else
            {
                PiCaster.App.bus.unlock_jack();
                PiCaster.App.bus.mic_button_active(false);
            }
        });
    }
}
