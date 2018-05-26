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

static gboolean
p_gstreamer_handle_message(GstBus *bus, GstMessage *message)
{
    g_print("handle_message - message type : %s\n", gst_message_type_get_name(GST_MESSAGE_TYPE(message)));
    return TRUE;
}

/*
static void
print_one_tag (const GstTagList * list, const gchar * tag, gpointer user_data)
{
  int i, num;

  num = gst_tag_list_get_tag_size (list, tag);
  for (i = 0; i < num; ++i) {
    const GValue *val;

    / * Note: when looking for specific tags, use the gst_tag_list_get_xyz() API,
     * we only use the GValue approach here because it is more generic * /
    val = gst_tag_list_get_value_index (list, tag, i);
    if (G_VALUE_HOLDS_STRING (val)) {
      g_print ("\t%20s : %s\n", tag, g_value_get_string (val));
    } else if (G_VALUE_HOLDS_UINT (val)) {
      g_print ("\t%20s : %u\n", tag, g_value_get_uint (val));
    } else if (G_VALUE_HOLDS_DOUBLE (val)) {
      g_print ("\t%20s : %g\n", tag, g_value_get_double (val));
    } else if (G_VALUE_HOLDS_BOOLEAN (val)) {
      g_print ("\t%20s : %s\n", tag,
          (g_value_get_boolean (val)) ? "true" : "false");
    } else if (GST_VALUE_HOLDS_BUFFER (val)) {
      GstBuffer *buf = gst_value_get_buffer (val);
      guint buffer_size = gst_buffer_get_size (buf);

      g_print ("\t%20s : buffer of size %u\n", tag, buffer_size);
    } else if (GST_VALUE_HOLDS_DATE_TIME (val)) {
      GstDateTime *dt = g_value_get_boxed (val);
      gchar *dt_str = gst_date_time_to_iso8601_string (dt);

      g_print ("\t%20s : %s\n", tag, dt_str);
      g_free (dt_str);
    } else {
      g_print ("\t%20s : tag of type '%s'\n", tag, G_VALUE_TYPE_NAME (val));
    }
  }
}
*/

static GstBusSyncReply*
p_gstreamer_handle_message_sync(GstBus* bus,GstMessage* message, GstElement* bin)
{
    if (message->type == GST_MESSAGE_TAG)
    {
        /*
        GstTagList *tags = NULL;
        gst_message_parse_tag (message, &tags);
        g_print ("Got tags from element %s:\n", GST_OBJECT_NAME (message->src));
        gst_tag_list_foreach (tags, print_one_tag, NULL);
        g_print ("\n");
        gst_tag_list_unref (tags);
        */
    }
    else
    /*
    if (message->type != GST_MESSAGE_TAG)
    */
    {
        g_print("handle_message_sync - %s : message type : %s\n", gst_element_get_name(bin), gst_message_type_get_name(GST_MESSAGE_TYPE(message)));
    }
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

static gboolean
cb_print_position(GstElement *pipeline)
{
    gint64 pos, len;

    if (gst_element_query_position (pipeline, GST_FORMAT_TIME, &pos) && gst_element_query_duration (pipeline, GST_FORMAT_TIME, &len)) {
        P_CALLBACK callback = (P_CALLBACK)g_object_get_data(G_OBJECT(pipeline), "callback");
        gpointer callback_user_data = g_object_get_data(G_OBJECT(pipeline), "callback_user_data");
        callback(pos, len, callback_user_data);
    }

    return TRUE;
}

GstElement*
p_gstreamer_play_track(gchar* file_path, P_CALLBACK callback, gpointer callback_user_data)
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
    g_object_set_data(G_OBJECT(bin), "callback", callback);
    g_object_set_data(G_OBJECT(bin), "callback_user_data", callback_user_data);
    g_timeout_add(500, (GSourceFunc)cb_print_position, bin);
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
