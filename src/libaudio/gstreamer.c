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

#include "libpicaster_audio.h"

static gboolean fx_player_active = FALSE;
static GstElement* fx_bin = NULL;

typedef enum
{
    GST_PLAY_FLAG_VIDEO         = (1 << 0), /* We want video output */
    GST_PLAY_FLAG_AUDIO         = (1 << 1), /* We want audio output */
    GST_PLAY_FLAG_TEXT          = (1 << 2)  /* We want subtitle output */
} GstPlayFlags;

static GstBusSyncReply
handle_message_sync_callback(GstBus* bus, GstMessage* message, GstElement* bin)
{
    if (message->type == GST_MESSAGE_EOS)
    {
        gint callback_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(bin), "callback_id"));
        g_source_remove(callback_id);
        PProgressCallback callback = (PProgressCallback)g_object_get_data(G_OBJECT(bin), "callback");
        gpointer callback_user_data = g_object_get_data(G_OBJECT(bin), "callback_user_data");
        callback(-1, -1, callback_user_data);
    }
    return GST_BUS_PASS;
}

void
libpicaster_gstreamer_init(int* p_argc, char*** p_argv)
{
    const gchar* name = "effects_player";

    fx_bin = gst_element_factory_make("playbin", name);
    gint flags;
    g_object_get(fx_bin, "flags", &flags, NULL);
    flags |= GST_PLAY_FLAG_AUDIO;
    flags &= ~GST_PLAY_FLAG_VIDEO;
    flags &= ~GST_PLAY_FLAG_TEXT;
    g_object_set(fx_bin, "flags", flags, NULL);

    GstElement* jack_sink = gst_element_factory_make("jackaudiosink", name);
    g_object_set(GST_OBJECT(jack_sink), "port_pattern", "in_effects_fader_*", "client-name", name, NULL);
    g_object_set(GST_OBJECT(fx_bin), "audio-sink", jack_sink, NULL);
    GstBus *bus = gst_element_get_bus(GST_ELEMENT(fx_bin));
    gst_bus_set_sync_handler(bus, (GstBusSyncHandler)handle_message_sync_callback, fx_bin, NULL);

    gst_element_set_state(fx_bin, GST_STATE_READY);
}

static gboolean
print_position_callback(GstElement *bin)
{
    gint64 pos, len;

    if (gst_element_query_position (bin, GST_FORMAT_TIME, &pos) && gst_element_query_duration (bin, GST_FORMAT_TIME, &len)) {
        PProgressCallback callback = (PProgressCallback)g_object_get_data(G_OBJECT(bin), "callback");
        gpointer callback_user_data = g_object_get_data(G_OBJECT(bin), "callback_user_data");
        callback(pos, len, callback_user_data);
    }

    return TRUE;
}

gpointer
p_gstreamer_play_fx(const char* file_path, PProgressCallback progress_callback, gpointer callback_user_data)
{
    if (fx_player_active)
    {
        return NULL;
    }
    fx_player_active = TRUE;
    gst_element_set_state(fx_bin, GST_STATE_READY);
    gchar* uri = g_strdup_printf("file://%s", file_path);
    g_object_set(fx_bin, "uri", uri, NULL);
    g_free(uri);
    g_object_set_data(G_OBJECT(fx_bin), "file_path", (gpointer)file_path);
    g_object_set_data(G_OBJECT(fx_bin), "callback", progress_callback);
    g_object_set_data(G_OBJECT(fx_bin), "callback_user_data", callback_user_data);
    gint callback_id = g_timeout_add(500, (GSourceFunc)print_position_callback, fx_bin);
    g_object_set_data(G_OBJECT(fx_bin), "callback_id", GINT_TO_POINTER(callback_id));
    gst_element_set_state(fx_bin, GST_STATE_PLAYING);
    return fx_bin;
}

void
p_gstreamer_stop_fx(gpointer audio_context)
{
    gst_element_set_state(fx_bin, GST_STATE_READY);
    fx_player_active = FALSE;
}
