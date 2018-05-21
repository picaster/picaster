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

#ifndef __CONTEXT_H_INCLUDED
#define __CONTEXT_H_INCLUDED

#include <gtk/gtk.h>
#include <jack/jack.h>

typedef struct {
    GtkBuilder*    builder;
    jack_client_t* jack_client;
    gboolean       jack_initialized;
    pid_t          jackd_pid;
    GApplication*  app;
    gboolean       microphone_opened;
    gdouble        dj_volume;
    gdouble        master_volume;
    gdouble        effects_volume;
} Context;

extern Context context;

void
p_context_init();

#endif
