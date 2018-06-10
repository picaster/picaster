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

#ifndef __JACK_FADER_MODULE_H_INCLUDED
#define __JACK_FADER_MODULE_H_INCLUDED

#include "JackModule.h"

class JackFaderModule : public JackModule
{

    private:
        double slider_value;
        float db_value;
        float amplification;
        bool enabled;

    public:
        JackFaderModule(const char* name, JackClient* client);
        void process(jack_nframes_t nframes);
        void setSliderValue(double slider_value);
        void enable(bool enabled);
};

#endif
