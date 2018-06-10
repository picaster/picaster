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

#include <gtk/gtk.h>
#include <alsa/asoundlib.h>

#include "context.h"
#include "device_list.h"

#include "AppSettingsManager.h"

extern "C" {
    void
    on_output_device_combo_box_text_changed(GtkComboBox* combo_box, gpointer user_data)
    {
        gchar* text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_box));
        AppSettingsManager::saveString("jack", "output_device", text);
        g_free(text);
        const gchar* id = gtk_combo_box_get_active_id(GTK_COMBO_BOX(combo_box));
        AppSettingsManager::saveString("jack", "output_device_id", id);
    }

    void
    on_input_device_combo_box_text_changed(GtkComboBox* combo_box, gpointer user_data)
    {
        gchar* text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_box));
        AppSettingsManager::saveString("jack", "input_device", text);
        g_free(text);
        const gchar* id = gtk_combo_box_get_active_id(GTK_COMBO_BOX(combo_box));
        AppSettingsManager::saveString("jack", "input_device_id", id);
    }
}

void
init_device_lists()
{
    GtkComboBoxText* input_device_combo_box = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(context.builder, "input_device_combo_box_text"));
    GtkComboBoxText* output_device_combo_box = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(context.builder, "output_device_combo_box_text"));

    gchar* input_device = AppSettingsManager::getString("jack", "input_device", NULL);
    gchar* output_device = AppSettingsManager::getString("jack", "output_device", NULL);

    int cardNum = -1;
    int err;
    for (;;) {
        snd_ctl_t *cardHandle;
        if ((err = snd_card_next(&cardNum)) < 0) {
            fprintf(stderr, "Can't get the next card number: %s\n", snd_strerror(err));
            break;
        }

        if (cardNum < 0) break;

        {
            char str[64];
            sprintf(str, "hw:%i", cardNum);
            if ((err = snd_ctl_open(&cardHandle, str, 0)) < 0)
            {
                printf("Can't open card %i: %s\n", cardNum, snd_strerror(err));
                continue;
            }
        }

        {
            snd_ctl_card_info_t *cardInfo;

            snd_ctl_card_info_alloca(&cardInfo);

            if ((err = snd_ctl_card_info(cardHandle, cardInfo)) < 0)
            {
                printf("Can't get info for card %i: %s\n", cardNum, snd_strerror(err));
            }
            else
            {
                const char* card_name = snd_ctl_card_info_get_name(cardInfo);
                const char* card_id = snd_ctl_card_info_get_id(cardInfo);
                gtk_combo_box_text_append(input_device_combo_box, card_id, card_name);
                gtk_combo_box_text_append(output_device_combo_box, card_id, card_name);

                if ((input_device != NULL) && (strcmp(card_name, input_device) == 0))
                {
                    gtk_combo_box_set_active(GTK_COMBO_BOX(input_device_combo_box), cardNum);
                }
                if ((output_device != NULL) && (strcmp(card_name, output_device) == 0))
                {
                    gtk_combo_box_set_active(GTK_COMBO_BOX(output_device_combo_box), cardNum);
                }
            }
        }
        snd_ctl_close(cardHandle);
    }

    snd_config_update_free_global();
    g_free(input_device);
    g_free(output_device);
}
