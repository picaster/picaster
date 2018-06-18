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

class FaderBox : Common.HorizontalBox
{
    public FaderBox()
    {
        var dj_fader = new Common.Fader("DJ", 0.0, (value) => {
            Common.Settings.save_string("faders", "dj", @"$value");
            PiCaster.App.bus.dj_fader_changed(value);
        });
        dj_fader.set_value(double.parse(Common.Settings.get_string("faders", "dj", "1.0"))); /* 0 dBFS */
        this.pack_start(dj_fader);

        this.pack_start(new Gtk.Separator(Gtk.Orientation.VERTICAL));

        var track_fader = new Common.Fader("TR", 0.0, (value) => {
            Common.Settings.save_string("faders", "track", @"$value");
            PiCaster.App.bus.track_fader_changed(value);
        });
        track_fader.set_value(double.parse(Common.Settings.get_string("faders", "track", "1.0"))); /* 0 dBFS */
        this.pack_start(track_fader);

        this.pack_start(new Gtk.Separator(Gtk.Orientation.VERTICAL));

        var fx_fader = new Common.Fader("FX", 0.0, (value) => {
            Common.Settings.save_string("faders", "fx", @"$value");
            PiCaster.App.bus.fx_fader_changed(value);
        });
        fx_fader.set_value(double.parse(Common.Settings.get_string("faders", "fx", "1.0"))); /* 0 dBFS */
        this.pack_start(fx_fader);

        this.pack_start(new Gtk.Separator(Gtk.Orientation.VERTICAL));

        var master_fader = new Common.Fader("MA", -3.0, (value) => {
            Common.Settings.save_string("faders", "master", @"$value");
            PiCaster.App.bus.master_fader_changed(value);
        });
        master_fader.set_value(double.parse(Common.Settings.get_string("faders", "master", "0.89917899646"))); /* -3 dBFS */
        this.pack_start(master_fader);
    }
}
