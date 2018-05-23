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

#include "ui.h"

#include <stdlib.h>

#include <gtk/gtk.h>
#include <alsa/asoundlib.h>

#include "../context.h"
#include "../settings.h"
#include "mumble.h"
#include "log.h"
#include "track_button.h"

static void
p_gtk_load_gui()
{
    const char* snap = getenv("SNAP");
    gchar* datadir = g_strdup_printf("%s%s", snap == NULL ? "" : snap, DATADIR);

    /* Load GUI */
    gchar* gui_file = g_strdup_printf("%s/%s", datadir, "picaster/desktop.glade");
    GtkBuilder* gtk_builder = gtk_builder_new_from_file(gui_file);

    /* Add CSS provider */
    GdkScreen* screen = gdk_screen_get_default();
    GtkCssProvider* provider = gtk_css_provider_new();
    gchar* css_file = g_strdup_printf("%s/%s", datadir, "picaster/desktop.css");
    p_log("Loading CSS from %s\n", css_file);
    gtk_css_provider_load_from_path(provider, css_file, NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_free(css_file);
    g_free(gui_file);
    g_free(datadir);

    context.builder = gtk_builder;
}

static void
p_gtk_show_main_window(GtkApplication *app)
{
    GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(context.builder, "mainwindow"));
    gtk_widget_set_size_request(GTK_WIDGET(window), 1000, 576);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_widget_show_all(window);
}

static void
p_gtk_init_device_lists()
{
    GtkComboBoxText* input_device_combo_box = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(context.builder, "input_device_combo_box_text"));
    GtkComboBoxText* output_device_combo_box = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(context.builder, "output_device_combo_box_text"));

    gchar* input_device = p_settings_get_string("jack", "input_device", NULL);
    gchar* output_device = p_settings_get_string("jack", "output_device", NULL);
    
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

static void
p_gtk_init_jack_settings()
{
    p_gtk_init_device_lists();
    gchar* jackd_path = p_settings_get_string("jack", "jackd_path_entry", "/usr/bin/jackd");
    gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(context.builder, "jackd_path_entry")), jackd_path);
}

static void
activate(GtkApplication *app, gpointer user_data)
{
    p_gtk_load_gui();
    p_gtk_init_device_lists();
    p_gtk_init_jack_settings();
    p_gtk_init_mumble_settings();
    p_gtk_init_track_buttons();

    p_context_subscribe(NOTIFICATION_JACK_STARTED, lambda(void, () {
        GtkBox* buttons_box = GTK_BOX(gtk_builder_get_object(context.builder, "buttons_box"));
        gtk_widget_set_sensitive(GTK_WIDGET(buttons_box), TRUE);
    }));
    p_context_subscribe(NOTIFICATION_JACK_STOPPED, lambda(void, () {
        GtkBox* buttons_box = GTK_BOX(gtk_builder_get_object(context.builder, "buttons_box"));
        gtk_widget_set_sensitive(GTK_WIDGET(buttons_box), FALSE);
    }));

    gtk_builder_connect_signals(context.builder, NULL);

    p_gtk_show_main_window(app);
}

GApplication*
p_gtk_init(int* p_argc, char*** p_argv)
{
    gtk_init(p_argc, p_argv);

    /* Create application */
    GtkApplication* app = gtk_application_new("ch.frenchguy.picaster", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    context.app = G_APPLICATION(app);

    return G_APPLICATION(app);
}

void
on_mainwindow_destroy(GtkWidget* widget)
{
    
}
