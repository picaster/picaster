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

#include "jack.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <gtk/gtk.h>
#include <jack/jack.h>
#include <jack/statistics.h>

#include "../context.h"
#include <config.h>

static jack_port_t* master_fader_input_ports[2];
static jack_port_t* master_fader_output_ports[2];
static jack_port_t* dj_fader_input_ports[2];
static jack_port_t* dj_fader_output_ports[2];
static jack_port_t* bedding_music_fader_input_ports[2];
static jack_port_t* bedding_music_fader_output_ports[2];
static jack_port_t* effects_fader_input_ports[2];
static jack_port_t* effects_fader_output_ports[2];
/*
static jack_port_t* recorder_input_ports[2];
*/

static gchar*
p_generate_jackdrc()
{
    GtkEntry* jackd_path_entry = GTK_ENTRY(gtk_builder_get_object(context.builder, "jackd_path_entry"));
    const gchar* jackd_path = gtk_entry_get_text(jackd_path_entry);

    //GtkSpinButton* priority_spin_button = GTK_SPIN_BUTTON(gtk_builder_get_object(context.builder, "priority_spin_button"));
    //gint priority = 99; //(gint)gtk_spin_button_get_value(priority_spin_button);

#ifdef USE_DUMMY_DRIVER
    gchar* command = g_strdup_printf("%s -ddummy", jackd_path);
#else
    GtkComboBoxText* sample_rate_combo_box = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(context.builder, "jack_samplerate_combobox_text"));
    const gchar* sample_rate = gtk_combo_box_get_active_id(GTK_COMBO_BOX(sample_rate_combo_box));

    //GtkComboBoxText* samples_per_period_combo_box = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(context.builder, "samples_per_period_combo_box"));
    const gchar* samples_per_period = "128"; //gtk_combo_box_get_active_id(GTK_COMBO_BOX(samples_per_period_combo_box));

    GtkComboBoxText* input_device_combo_box_text = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(context.builder, "input_device_combo_box_text"));
    const gchar* input_device = gtk_combo_box_get_active_id(GTK_COMBO_BOX(input_device_combo_box_text));

    GtkComboBoxText* output_device_combo_box_text = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(context.builder, "output_device_combo_box_text"));
    const gchar* output_device = gtk_combo_box_get_active_id(GTK_COMBO_BOX(output_device_combo_box_text));

    gchar* command = g_strdup_printf("%s -dalsa -r%s -p%s -n4 -D -Chw:%s -Phw:%s", jackd_path, sample_rate, samples_per_period, input_device, output_device);
#endif
    gchar* jackdrc = g_strdup_printf("%s/.jackdrc", getenv("HOME"));
    FILE* fp = fopen(jackdrc, "w");
    fputs(command, fp);
    fflush(fp);
    fclose(fp);
    g_free(jackdrc);
    //g_free(command);
    return command;
}

void
p_jack_start_server()
{
    pid_t pid = fork();
    if (pid > 0) {
        context.jackd_pid = pid;
    }
    else if (pid == 0)
    {
        // We are the child
        gchar* command_line = p_generate_jackdrc();
        gchar** tokens = g_strsplit(command_line, " ", -1);
        g_free(command_line);

        //execl("/usr/bin/jackd", "/usr/bin/jackd", "-dalsa", "-r44100", "-p128", "-n4", "-D", "-Chw:CODEC", "-Phw:CODEC", NULL);
        execv(tokens[0], tokens);

        // just in case, but we're gone at this point.
        g_strfreev(tokens);
    }
    else
    {
        // We shall never get here... right ?
    }
}

static void
p_jack_init_master_fader()
{
    master_fader_input_ports[0] = jack_port_register(context.jack_client, "in_master_fader_1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    master_fader_input_ports[1] = jack_port_register(context.jack_client, "in_master_fader_2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    master_fader_output_ports[0] = jack_port_register(context.jack_client, "out_master_fader_1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    master_fader_output_ports[1] = jack_port_register(context.jack_client, "out_master_fader_2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

    const char** ports = jack_get_ports(context.jack_client, "playback*", NULL, JackPortIsPhysical|JackPortIsInput);

    jack_connect(context.jack_client, jack_port_name(master_fader_output_ports[0]), ports[0]);
    jack_connect(context.jack_client, jack_port_name(master_fader_output_ports[1]), ports[1]);

    jack_free(ports);
}

static void
p_jack_init_dj_fader()
{
    const char** ports = jack_get_ports(context.jack_client, "capture*", NULL, JackPortIsPhysical|JackPortIsOutput);

    dj_fader_input_ports[0] = jack_port_register(context.jack_client, "in_dj_fader_1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    dj_fader_input_ports[1] = jack_port_register(context.jack_client, "in_dj_fader_2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    dj_fader_output_ports[0] = jack_port_register(context.jack_client, "out_dj_fader_1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    dj_fader_output_ports[1] = jack_port_register(context.jack_client, "out_dj_fader_2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

    jack_connect(context.jack_client, ports[0], jack_port_name(dj_fader_input_ports[0]));
    jack_connect(context.jack_client, ports[1], jack_port_name(dj_fader_input_ports[1]));
    jack_connect(context.jack_client, jack_port_name(dj_fader_output_ports[0]), jack_port_name(master_fader_input_ports[0]));
    jack_connect(context.jack_client, jack_port_name(dj_fader_output_ports[1]), jack_port_name(master_fader_input_ports[1]));
    jack_free(ports);
}

static void
p_jack_init_effects_fader()
{
    effects_fader_input_ports[0] = jack_port_register(context.jack_client, "in_effects_1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    effects_fader_input_ports[1] = jack_port_register(context.jack_client, "in_effects_2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    effects_fader_output_ports[0] = jack_port_register(context.jack_client, "out_effects_1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    effects_fader_output_ports[1] = jack_port_register(context.jack_client, "out_effects_2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

    jack_connect(context.jack_client, jack_port_name(effects_fader_output_ports[0]), jack_port_name(master_fader_input_ports[0]));
    jack_connect(context.jack_client, jack_port_name(effects_fader_output_ports[1]), jack_port_name(master_fader_input_ports[1]));
}

static void
p_jack_init_bedding_music_fader()
{
    bedding_music_fader_input_ports[0] = jack_port_register(context.jack_client, "in_bedding_music_1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    bedding_music_fader_input_ports[1] = jack_port_register(context.jack_client, "in_bedding_music_2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    bedding_music_fader_output_ports[0] = jack_port_register(context.jack_client, "out_bedding_music_1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    bedding_music_fader_output_ports[1] = jack_port_register(context.jack_client, "out_bedding_music_2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

    jack_connect(context.jack_client, jack_port_name(bedding_music_fader_output_ports[0]), jack_port_name(master_fader_input_ports[0]));
    jack_connect(context.jack_client, jack_port_name(bedding_music_fader_output_ports[1]), jack_port_name(master_fader_input_ports[1]));
}

static void
p_jack_shutdown (void *arg)
{
    g_print("p_jack_shutdown : we should probably do something here...\n");
}

void
p_stop_jack_server()
{
    kill(context.jackd_pid, SIGINT);
}

static void
p_jack_signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        p_stop_jack_server();
        g_application_release(context.app);
    }
    g_print("p_jack_signal_handler : sig = %d - we should probably do something here...\n", sig);
}

static int
p_jack_process(jack_nframes_t nframes, void* user_data)
{
    if (context.jack_initialized)
    {
        for (int i = 0; i < 2; i++)
        {
            jack_default_audio_sample_t* master_in = jack_port_get_buffer(master_fader_input_ports[i], nframes);
            jack_default_audio_sample_t* master_out = jack_port_get_buffer(master_fader_output_ports[i], nframes);
            jack_default_audio_sample_t* dj_in = jack_port_get_buffer(dj_fader_input_ports[i], nframes);
            jack_default_audio_sample_t* dj_out = jack_port_get_buffer(dj_fader_output_ports[i], nframes);
            jack_default_audio_sample_t* effects_in = jack_port_get_buffer(effects_fader_input_ports[i], nframes);
            jack_default_audio_sample_t* effects_out = jack_port_get_buffer(effects_fader_output_ports[i], nframes);
            jack_default_audio_sample_t* bedding_music_in = jack_port_get_buffer(bedding_music_fader_input_ports[i], nframes);
            jack_default_audio_sample_t* bedding_music_out = jack_port_get_buffer(bedding_music_fader_output_ports[i], nframes);

            for (int i = 0; i < nframes; i++)
            {
                master_out[i] = master_in[i] * context.master_volume;
                dj_out[i] = context.microphone_opened ? dj_in[i] * context.dj_volume : 0;
                effects_out[i] = effects_in[i] * context.effects_volume;
                bedding_music_out[i] = bedding_music_in[i];
            }
        }        
    }
    return 0;
}

static int
p_jack_xrun()
{
    float delay = jack_get_max_delayed_usecs(context.jack_client);
    g_print("xrun detected : %f uS\n", delay);
    return 0;
}

void
p_jack_init_client()
{
    jack_status_t jack_status = 0;
    jack_options_t options = JackNoStartServer;
    jack_client_t* client = jack_client_open("PiCaster", options, &jack_status, NULL);
    if (client == NULL)
    {
        g_error("Overall operation failed.\n");
        exit(1);
    }
    if (jack_status & JackServerStarted)
    {
        g_print("JACK server started\n");
    }
    if (jack_status & JackNameNotUnique)
    {
        g_print("Unique name '%s' assigned\n", jack_get_client_name(client));
    }

    jack_on_shutdown(client, p_jack_shutdown, 0);

    jack_set_process_callback(client, p_jack_process, NULL);

    jack_set_xrun_callback(client, p_jack_xrun, NULL);

    if (jack_activate(client))
    {
        g_error("cannot activate client\n");
    }

    context.jack_client = client;

    p_jack_init_master_fader();
    p_jack_init_dj_fader();
    p_jack_init_bedding_music_fader();
    p_jack_init_effects_fader();

    signal(SIGQUIT, p_jack_signal_handler);
    signal(SIGTERM, p_jack_signal_handler);
    signal(SIGHUP, p_jack_signal_handler);
    signal(SIGINT, p_jack_signal_handler);
}
