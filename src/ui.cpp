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

#include <stdlib.h>

#include "context.h"
#include "ui.h"
#include "device_list.h"
#include "jack.h"

static void
load_gui()
{
    /* Load glade file. */
    const char* snap = getenv("SNAP");
    gchar* datadir = g_strdup_printf("%s%s", snap == NULL ? "" : snap, DATADIR);
    gchar* gui_file = g_strdup_printf("%s/picaster/desktop.glade", datadir);
    context.builder = gtk_builder_new_from_file(gui_file);

    /* Add our own CSS provider */
    GdkScreen* screen = gdk_screen_get_default();
    GtkCssProvider* provider = gtk_css_provider_new();
    gchar* css_file = g_strdup_printf("%s/picaster/desktop.css", datadir);
    gtk_css_provider_load_from_path(provider, css_file, NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_free(css_file);
    g_free(gui_file);
    g_free(datadir);
}

void
activate(GtkApplication* app, gpointer user_data)
{
    load_gui();
    init_device_lists();
    init_jack_settings();

    if (context.jack_client->isConnected())
    {
        GtkWidget* jack_box = GTK_WIDGET(gtk_builder_get_object(context.builder, "jack_box"));
        gtk_widget_set_sensitive(jack_box, false);
    }

    gtk_builder_connect_signals(context.builder, NULL);

    GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(context.builder, "mainwindow"));
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_widget_show_all(window);
}

