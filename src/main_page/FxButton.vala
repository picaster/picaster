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

class FxButton : BaseMusicButton
{
    private int index;

    public FxButton(int index)
    {
        base(@"FX$index");

        this.index = index;

        this.add_accelerator("accel", PiCaster.App.accel_group, Gdk.Key.@0 + index, Gdk.ModifierType.CONTROL_MASK, Gtk.AccelFlags.VISIBLE);

        PiCaster.App.bus.lock_fx_button.connect(() => 
        {
            if (!button.active)
                sensitive = false;
        });
        
        PiCaster.App.bus.unlock_fx_button.connect(() => 
        {
            sensitive = true;
        });
    }

    protected override void play()
    {
        PiCaster.App.bus.lock_fx_button();
    }

    protected override void stop()
    {
        PiCaster.App.bus.unlock_fx_button();
    }

}
