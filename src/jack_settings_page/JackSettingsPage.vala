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

namespace PiCaster
{ 
    public class JackSettingsPage : Gtk.Grid
    {
        private int row;

        public JackSettingsPage()
        {
            this.column_spacing = 4;
            this.row_spacing = 4;
            this.margin_top = 4;
            this.margin_bottom = 4;
            this.margin_start = 4;
            this.margin_end = 4;
            this.row = 0;

            // jackd
            var jackd_path_entry = new Gtk.Entry();
            jackd_path_entry.text = Common.Settings.get_string("jack", "jackd_path_entry", "/usr/bin/jackd");
            jackd_path_entry.changed.connect(() => {
                Common.Settings.save_string("jack", "jackd_path_entry", jackd_path_entry.text);
            });
            add_field("Path to jackd", jackd_path_entry);

            // driver
            var driver_combobox = new Gtk.ComboBoxText();
            driver_combobox.append("alsa", "ALSA");
            driver_combobox.append("dummy", "Dummy driver");
            driver_combobox.active_id = Common.Settings.get_string("jack", "driver", "alsa");
            driver_combobox.changed.connect(() => {
                Common.Settings.save_string("jack", "driver", driver_combobox.active_id);
            });
            this.add_field("Driver", driver_combobox);

            // input device
            var input_device_combobox = new Gtk.ComboBoxText();
            this.add_field("Input device", input_device_combobox);
            input_device_combobox.changed.connect(() => {
                Common.Settings.save_string("jack", "input_device_id", input_device_combobox.active_id);
            });

            // output device
            var output_device_combobox = new Gtk.ComboBoxText();
            this.add_field("Output device", output_device_combobox);
            output_device_combobox.changed.connect(() => {
                Common.Settings.save_string("jack", "output_device_id", output_device_combobox.active_id);
            });

            int card_num = 0;
            int err;
            for (;;) {
                Alsa.Card card;
                if ((err = Alsa.Card.open(out card, @"hw:$(card_num)", 0)) < 0)
                {
                    // We assume that if we can't open that card, then we have enumerated all of them, so we exit.
                    break;
                }

                Alsa.CardInfo card_info;
                Alsa.CardInfo.alloc(out card_info);

                if ((err = card.card_info(card_info)) < 0)
                {
                    stderr.printf("Can't get info for card %i: %s\n", card_num, Alsa.strerror(err));
                }
                else
                {
                    string card_name = card_info.get_name();
                    string card_id = card_info.get_id();
                    input_device_combobox.append(card_id, card_name);
                    output_device_combobox.append(card_id, card_name);
                }

                card_num += 1;
            }
            input_device_combobox.active_id = Common.Settings.get_string("jack", "input_device_id", "");
            output_device_combobox.active_id = Common.Settings.get_string("jack", "output_device_id", "");

            // sample rate
            var sample_rate_combobox = new Gtk.ComboBoxText();
            sample_rate_combobox.append("48000", "48000");
            sample_rate_combobox.append("44100", "44100");
            sample_rate_combobox.active_id = Common.Settings.get_string("jack", "sample_rate", "48000");
            sample_rate_combobox.changed.connect(() => {
                Common.Settings.save_string("jack", "sample_rate", sample_rate_combobox.active_id);
            });
            this.add_field("Sample rate (Hz)", sample_rate_combobox);

            // frames per period
            var frames_per_period_combobox = new Gtk.ComboBoxText();
            frames_per_period_combobox.append("16", "16");
            frames_per_period_combobox.append("32", "32");
            frames_per_period_combobox.append("64", "64");
            frames_per_period_combobox.append("128", "128");
            frames_per_period_combobox.append("256", "256");
            frames_per_period_combobox.append("512", "512");
            frames_per_period_combobox.append("1024", "1024");
            frames_per_period_combobox.append("2048", "2048");
            frames_per_period_combobox.append("4096", "4096");
            frames_per_period_combobox.active_id = Common.Settings.get_string("jack", "frames_per_period", "256");
            frames_per_period_combobox.changed.connect(() => {
                Common.Settings.save_string("jack", "frames_per_period", frames_per_period_combobox.active_id);
            });
            this.add_field("Frames per period", frames_per_period_combobox);

            // periods per buffer
            var spin = new Gtk.SpinButton.with_range(1, 9, 1);
            spin.numeric = true;
            spin.value = double.parse(Common.Settings.get_string("jack", "periods_per_buffer", "3"));
            spin.value_changed.connect(() => {
                Common.Settings.save_string("jack", "periods_per_buffer", @"$(spin.value)");
            });
            this.add_field("Periods per buffer", spin);

        }

        private void add_field(string label_text, Gtk.Widget field)
        {
            var label = new Gtk.Label(label_text + " :");
            label.justify = Gtk.Justification.RIGHT;
            this.attach(label, 0, this.row);
            this.attach(field, 1, this.row);
            this.row += 1;
        }
    }
}