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

#include <unistd.h>
#include <iostream>
#include <assert.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "context.h"
#include "ui.h"
#include "jack.h"

int
main(int argc, char** argv)
{
    context.jack_client = JackClient::getInstance("PiCaster");

    if (context.jack_client->isConnected())
    {
        init_jack_modules();
    }

    /* Init GTK */
    gtk_init(&argc, &argv);

    /* Create application */
    GtkApplication* app = gtk_application_new("ch.frenchguy.picaster", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    /* Run application */
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    return status;
}
