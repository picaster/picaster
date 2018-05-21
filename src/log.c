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

#include "log.h"

#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdarg.h>

void p_log_init()
{
    openlog("picaster", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}

void
p_log(const char* format, ...)
{

    va_list argptr;
    va_start(argptr, format);
    syslog(LOG_NOTICE, format, argptr);
    va_end(argptr);
}

void
p_log_close()
{
    closelog();
}
