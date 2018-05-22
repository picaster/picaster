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

#include "gstreamer.h"

#include <string.h>

#include "../context.h"

static GstElement* bins[4];
static gboolean player_a_active = FALSE;
static gboolean player_b_active = FALSE;

/* playbin flags */
typedef enum
{
    GST_PLAY_FLAG_VIDEO         = (1 << 0), /* We want video output */
    GST_PLAY_FLAG_AUDIO         = (1 << 1), /* We want audio output */
    GST_PLAY_FLAG_TEXT          = (1 << 2)  /* We want subtitle output */
} GstPlayFlags;

static void
p_gstreamer_handle_message(GstBus *bus, GstMessage *message)
{
    g_print("handle_message - message type : %s\n", gst_message_type_get_name(GST_MESSAGE_TYPE(message)));
}

static GstBusSyncReply*
p_gstreamer_handle_message_sync(GstBus* bus,GstMessage* message, GstElement* bin)
{
    /*
    if (message->type != GST_MESSAGE_TAG)
    {
        g_print("handle_message_sync - %s : message type : %s\n", gst_element_get_name(bin), gst_message_type_get_name(GST_MESSAGE_TYPE(message)));
    }
    */
    if (message->type == GST_MESSAGE_EOS)
    {/*
        GtkToggleButton* button = GTK_TOGGLE_BUTTON(g_object_get_data(G_OBJECT(bin), "button"));
        gtk_toggle_button_set_active(button, FALSE);*/
    }
    return NULL;
}

void
p_gstreamer_init(int* argc, char*** argv)
{
    /* Initialize GStreamer */
    gst_init(argc, argv);

    /* Create the elements */
    bins[0] = gst_element_factory_make("playbin", "track_player_a");
    bins[1] = gst_element_factory_make("playbin", "track_player_b");
    bins[2] = gst_element_factory_make("playbin", "effects_player");
    bins[3] = gst_element_factory_make("playbin", "bedding_music_player");

    /* Set flags */
    gint flags;
    for (int i = 0; i < 4; i++)
    {
        GstElement* bin = bins[i];
        gchar* name = gst_element_get_name(bin);
        g_object_get(bin, "flags", &flags, NULL);
        flags |= GST_PLAY_FLAG_AUDIO;
        flags &= ~GST_PLAY_FLAG_VIDEO;
        flags &= ~GST_PLAY_FLAG_TEXT;
        g_object_set(bin, "flags", flags, NULL);
        /* Set bin's audio sink to be our jackaudiosink */
        GstElement* jack_sink = gst_element_factory_make("jackaudiosink", name);
        gchar* pattern = g_strdup_printf("in_%s_fader_*", (i == 3) ? "bedding_music" : ((i == 2) ? "effects" : "master"));
        g_object_set(GST_OBJECT(jack_sink), "port_pattern", pattern, "client-name", name, NULL);
        g_free(pattern);
        g_object_set(GST_OBJECT(bin), "audio-sink", jack_sink, NULL);
        GstBus *bus = gst_element_get_bus(GST_ELEMENT(bin));
        gst_bus_add_watch(bus, (GstBusFunc)p_gstreamer_handle_message, NULL);
        gst_bus_set_sync_handler(bus, (GstBusSyncHandler)p_gstreamer_handle_message_sync, bin, NULL);
        /* Pause */
        GstStateChangeReturn ret = gst_element_set_state(bin, GST_STATE_READY);
        if (ret == GST_STATE_CHANGE_FAILURE)
        {
            g_error("Unable to set the pipeline to the paused state.\n");
        }

        g_free(name);
    }
}

GstElement*
p_gstreamer_play_track(gchar* file_path)
{
    if (player_a_active && player_b_active)
    {
        return NULL;
    }
    GstElement* bin;
    if (player_a_active)
    {
        bin = bins[1];
        player_b_active = TRUE;
    }
    else
    {
        bin = bins[0];
        player_a_active = TRUE;
    }
    gchar* uri = g_strdup_printf("file://%s", file_path);
    gst_element_set_state(bin, GST_STATE_READY);
    g_object_set(bin, "uri", uri, NULL);
    g_object_set_data(G_OBJECT(bin), "file_path", file_path);
    g_free(uri);
    gst_element_set_state(bin, GST_STATE_PLAYING);
    return bin;
}

void
p_gstreamer_stop_track(gpointer audio_context)
{
    GstElement* bin = GST_ELEMENT(audio_context);
    gst_element_set_state(bin, GST_STATE_READY);
    gchar* name = gst_element_get_name(bin);
    if (!strcmp(name, "track_player_a"))
    {
        player_a_active = FALSE;
    }
    else
    {
        player_b_active = FALSE;
    }
    g_free(name);
}