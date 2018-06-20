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

public class TrackButton : BaseMusicButton
{
    private int index;
    private static int count;

    public TrackButton(int index)
    {
        base(@"TRK$index");

        this.index = index;

        var progress_bar = new Gtk.ProgressBar();
        progress_bar.set_show_text(false);
        progress_bar.set_fraction(0.0);
        this.pack_end(progress_bar);

        this.add_accelerator("accel", PiCaster.App.accel_group, Gdk.Key.@0 + index, 0, Gtk.AccelFlags.VISIBLE);

        TrackButton.count = 0;

        PiCaster.App.bus.lock_track_button.connect(() => 
        {
            if (!button.active)
                sensitive = false;
        });
        
        PiCaster.App.bus.unlock_track_button.connect(() => 
        {
            sensitive = true;
        });
    }

    protected override void play()
    {
        TrackButton.count += 1;
        if (TrackButton.count > 1)
        {
            PiCaster.App.bus.lock_track_button();
        }
    }

    protected override void stop()
    {
        TrackButton.count -= 1;
        if (TrackButton.count < 2)
        {
            PiCaster.App.bus.unlock_track_button();
        }
    }
}
