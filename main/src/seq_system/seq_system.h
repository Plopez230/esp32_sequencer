/*************************************************************************************
  Copyright (c) 2022, plopez230@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
**************************************************************************************/

#ifndef SEQ_SYSTEM_H
# define SEQ_SYSTEM_H



/**
 * Event structure
 */
typedef struct    s_seq_event
{
  uint32_t        idle_ticks;
  uint8_t         message[SEQ_CONFIG_MAX_MESSAGE_LENGTH];
}                 t_seq_event;

typedef struct s_seq_ring {} t_seq_ring;
typedef struct hw_timer_s {} hw_timer_t;

/**
 * Sequencer track structure
 */
typedef struct    s_seq_track
{
  t_seq_ring      ring;
  cppQueue        *track_buffer;
  uint16_t        elapsed_ticks;
  uint16_t        idle_ticks;
  uint8_t         enable;
  t_seq_event     next_event;
}                 t_seq_track;

/**
 * Sequencer structure
 */
typedef struct    s_seq_sequencer
{
  t_seq_track     *tracks[SEQ_CONFIG_MAX_TRACKS];
  uint16_t        idle_ticks;
  uint16_t        elapsed_ticks;
  uint8_t         enable;
  hw_timer_t      *timer;
  t_seq_ring      *output_buffer;
}                 t_seq_sequencer;

/**
 * Synth tuning structure
 */
typedef struct        s_seq_synth_tuning
{
  uint16_t            f_numbers[36];
  uint8_t             divisions_per_octave; 
}                     t_seq_synth_tuning;

/**
 * Synth channel structure
 */
typedef struct        s_seq_synth_channel
{
  uint8_t             instrument;
  uint8_t             note;
  uint8_t             velocity;
  uint8_t             used;
  uint64_t            last_active;
}                     t_seq_synth_channel;

typedef struct        s_seq_midi_channel
{
  uint8_t             poly_mode;
  uint8_t             sustain;
  t_seq_synth_tuning  tuning;
  uint8_t             instrument;
  uint8_t             volume;
  uint8_t             mute;
}                     t_seq_midi_channel;

typedef struct        s_seq_fm_preset
{
  int8_t             carrier_ksl;
  int8_t             carrier_rel;
  int8_t             carrier_mul;
  int8_t             carrier_sus;
  int8_t             carrier_att;
  int8_t             carrier_dec;
  int8_t             carrier_am;
  int8_t             carrier_ksr;
  int8_t             carrier_dc;
  int8_t             carrier_vib;
  int8_t             carrier_eg;
  int8_t             dc;
  int8_t             dm;
  int8_t             modulator_ksl;
  int8_t             modulator_rel;
  int8_t             modulator_mul;
  int8_t             modulator_sus;
  int8_t             modulator_att;
  int8_t             modulator_dec;
  int8_t             modulator_am;
  int8_t             modulator_ksr;
  int8_t             modulator_dc;
  int8_t             modulator_vib;
  int8_t             modulator_eg;
  int8_t             feedback;
  int8_t             level;
}                     t_seq_fm_preset;

typedef struct        s_seq_midi_patch
{
    t_seq_midi_channel midi_channels[16];
    t_seq_fm_preset    original_instrument;
    uint8_t            original_instrument_register[8];
}                     t_seq_midi_patch;

typedef struct        s_seq_synth
{
  t_seq_synth_channel channels[9];
  t_seq_midi_patch    patch;
  cppQueue            *input_buffer;
  t_seq_synth_tuning  *tuning;
  //MD_YM2413           *synth_chip;
  uint8_t             last_active_channel;
}                     t_seq_synth;

typedef struct        s_seq_console
{
  cppQueue            *input_buffer;
  TaskHandle_t        task_handler;
  char                print_buffer[SEQ_CONFIG_CONSOLE_PRINT_BUFFER_MAX_SIZE];
  char                *command_buffer;
  char                *return_buffer;
  uint32_t            print_line_cursor;
}                     t_seq_console;

typedef struct        s_seq_loop_sequence
{
  char                sequence_file[SEQ_CONFIG_MAX_FILE_NAME_SIZE];
  uint8_t             start_sequence_idx;
  uint8_t             start_sequence_mode;
  int64_t             start_sequence_displacement;
  uint8_t             end_sequence_idx;
  uint8_t             end_sequence_mode;
  int64_t             end_sequence_displacement;
  uint8_t             sequence_alignment;
  uint8_t             sequence_repeat;
}                     t_seq_loop_sequence;

uint32_t str_length_up_to_newline(char *str);
char *previous_line_start(char *str, char *pos);
char *get_full_line(char *str, char *pos, char **last_line_start, uint32_t *last_line_length);
char *print_last_lines(char *test, char *temp, uint32_t n_lines, uint32_t offset);
t_seq_console *seq_console_create();
void seq_draw_command(t_seq_console *console);
void seq_draw_status_bar();
void seq_console_draw();
void seq_console_task(void *parameter);
void seq_console_init_task(t_seq_console *console);
void seq_console_init(t_seq_console *console);

void seq_track_delete(t_seq_track *track);
t_seq_track *seq_track_create();
t_seq_sequencer *seq_sequencer_create();
uint64_t  seq_track_update(t_seq_sequencer *sequencer, t_seq_track *track, uint32_t delta);
void IRAM_ATTR onTimer();
void seq_sequencer_setup_timer(t_seq_sequencer *sequencer, uint16_t bpm, uint16_t precision);
uint64_t seq_sequencer_update(t_seq_sequencer *sequencer, uint32_t delta);
void  seq_sequencer_loop(t_seq_sequencer *sequencer);

t_seq_synth *seq_synth_create();
void seq_synth_reset_channels(t_seq_synth *synth);
int8_t  seq_synth_get_channel2(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity);
int8_t  seq_synth_get_channel(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity);
int8_t  seq_synth_release_channel(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity);
int note_(int note);
int octave_ (int note);
void seq_synth_play_note(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity);
void seq_synth_stop_note(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity);
void seq_synth_init(t_seq_synth *synth);
void seq_synth_loop(t_seq_synth *synth);

void seq_tuning_equal_tempered(t_seq_synth_tuning *tuning, uint16_t divisions, uint16_t base_frequency);
void seq_tuning_12(t_seq_synth_tuning *tuning);
void seq_tuning_13(t_seq_synth_tuning *tuning);
void seq_tuning_14(t_seq_synth_tuning *tuning);
void seq_tuning_15(t_seq_synth_tuning *tuning);
void seq_tuning_16(t_seq_synth_tuning *tuning);
void seq_tuning_17(t_seq_synth_tuning *tuning);

void seq_register_commands();

#endif