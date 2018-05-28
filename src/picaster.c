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

#include "picaster.h"

#include "ui/ui.h"
#include "log.h"
#include "context.h"
#include "audio/gstreamer.h"

#include "dummy-audio.h"
#include "dummy-mumble.h"
#include "libaudio/libpicaster_audio.h"

int
main(int argc, char** argv)
{
    dummy_audio();
    dummy_mumble();

    p_log_init();
    p_log("Starting picaster");

    p_context_init();

    p_gstreamer_init(&argc, &argv);

    libpicaster_audio_init(&argc, &argv);

    GApplication* application = p_gtk_init(&argc, &argv);
    int status = g_application_run(application, argc, argv);

    p_log("Stoping picaster");
    p_log_close();

    return status;
}