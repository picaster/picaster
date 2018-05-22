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

#include "context.h"

#include <string.h>

Context context;

static GHashTable* notifications = NULL;

void
p_context_init()
{
    memset(&context, 0, sizeof(Context));
    context.dj_volume = 1.0f;
    context.master_volume = 1.0f;
    context.effects_volume = 1.0f;
    notifications = g_hash_table_new(g_str_hash, g_str_equal);
}

void
p_context_notify(const gchar* notification)
{
    GList* callbacks = (GList*)g_hash_table_lookup(notifications, notification);
    if (callbacks != NULL)
    {
        for (GList* l = callbacks; l != NULL; l = l->next)
        {
            void(*callback)() = (void(*)())l->data;
            callback();
        }
    }
}

void
p_context_subscribe(gchar* notification, void(*callback)())
{
    GList* callbacks = (GList*)g_hash_table_lookup(notifications, notification);
    if (callbacks == NULL)
    {
        callbacks = g_list_prepend(callbacks, callback);
        g_hash_table_insert(notifications, notification, callbacks);
    }
    else
    {
        callbacks = g_list_prepend(callbacks, callback);
        g_hash_table_replace(notifications, notification, callbacks);
    }
}