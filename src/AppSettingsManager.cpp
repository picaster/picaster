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

#include "AppSettingsManager.h"

#define PICASTER_INI_FILE "picaster.ini"

gboolean
AppSettingsManager::getKeyFile(GKeyFile** key_file, gchar** full_path)
{
    *key_file = g_key_file_new();
    gboolean ret = g_key_file_load_from_data_dirs(*key_file, PICASTER_INI_FILE, full_path, G_KEY_FILE_KEEP_COMMENTS, NULL);
    if ((full_path != NULL) && (*full_path == NULL))
    {
        *full_path = g_strdup_printf("%s/%s", g_get_user_data_dir(), PICASTER_INI_FILE);
    }
    return ret;    
}

gchar*
AppSettingsManager::getString(const gchar* section, const gchar* key, const gchar* default_value)
{
    GKeyFile* key_file;
    GError* error = NULL;
    AppSettingsManager::getKeyFile(&key_file, NULL);
    gchar* value = g_key_file_get_string(key_file, section, key, &error);
    g_key_file_free(key_file);
    if (error == NULL)
    {
        return value;
    }
    else
    {
        return g_strdup(default_value);
    }
}

void
AppSettingsManager::saveString(const gchar* section, const gchar* key, const gchar* value)
{
    gchar* full_path = NULL;
    GKeyFile* key_file;
    AppSettingsManager::getKeyFile(&key_file, &full_path);
    g_key_file_set_string(key_file, section, key, value);
    g_key_file_save_to_file(key_file, full_path, NULL);
    g_print("settings saved %s\n", full_path);
    g_free(full_path);
    g_key_file_free(key_file);
}
