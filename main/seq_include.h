/*************************************************************************************
  Copyright (c) 2022, plopez230@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice, this
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

#ifndef SEQ_INCLUDE_H
# define SEQ_INCLUDE_H

# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <cppQueue.h>
# include <FreeRTOS_CLI_vESP32.h>
# include <U8g2lib.h>
# include "Adafruit_MPR121.h"
# include "seq_configuration.h"
# include "seq_structures.h"
# include "seq_peripheral.h"

/**
   System global variable.
*/
extern t_seq_system seq_system;
extern SEQ_U8G2_CLASS u8g2;

/**
   seq_st7920.cpp prototypes
*/
void        seq_st7920_prepare(void);
void        seq_st7920_init(void);
void        seq_st7920_draw(void (*draw_callback)());

/**
   seq_mpr121.cpp prototypes
*/
void        seq_mpr121_init(void);
void        seq_mpr121_get_events(void);

/**
   seq_ring.cpp prototypes
*/
int         seq_ring_init(t_seq_ring *ring, uint32_t element_size,
                          uint32_t max_elements);
uint32_t    seq_ring_elements_used(t_seq_ring *ring);
uint32_t    seq_ring_bytes_used(t_seq_ring *ring);
uint32_t    seq_ring_elements_free(t_seq_ring *ring);
uint32_t    seq_ring_bytes_free(t_seq_ring *ring);
int         seq_ring_push(t_seq_ring *ring, void *src);
int         seq_ring_pop(t_seq_ring *ring, void *dst);

/**
   seq_mpr121.cpp prototypes
*/
t_seq_keyboard *seq_keyboard_create();
void seq_keyboard_init(t_seq_keyboard *keyboard);
void seq_keyboard_device_touched(t_seq_keyboard *keyboard, Adafruit_MPR121 *seq_mpr121, uint16_t *last_touched, uint8_t key_offset);
void seq_keyboard_instrument_press(t_seq_keyboard *keyboard, uint8_t key);
void seq_keyboard_instrument_release(t_seq_keyboard *keyboard, uint8_t key);
void seq_keyboard_set_instrument_mode(t_seq_keyboard *keyboard);
void seq_keyboard_control_press(t_seq_keyboard *keyboard, uint8_t key);
void seq_keyboard_control_release(t_seq_keyboard *keyboard, uint8_t key);
void seq_keyboard_set_control_mode(t_seq_keyboard *keyboard);
void seq_keyboard_update_mode(t_seq_keyboard *keyboard);
void seq_keyboard_text_press(t_seq_keyboard *keyboard, uint8_t key);
void seq_keyboard_text_release(t_seq_keyboard *keyboard, uint8_t key);
void seq_keyboard_set_text_mode(t_seq_keyboard *keyboard);
void seq_keyboard_loop(t_seq_keyboard *keyboard);
void seq_keyboard_check_mode_button(t_seq_keyboard *keyboard);


t_seq_synth *seq_synth_create();
void seq_ym2413_shift (uint8_t value);
void seq_synth_reset_channels(t_seq_synth *synth);
int8_t  seq_synth_get_channel(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity);
int8_t  seq_synth_release_channel(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity);
void seq_ym2413_setup_pins();
void seq_ym2413_select_chip();
void seq_ym2413_reset_chip();
void seq_ym2413_setup();
void seq_ym2413_write(byte address, byte data);
void seq_ym2413_play_note(s_seq_synth_tuning *tuning, int channel, int note, int instrument, int vol);
void seq_ym2413_stop_note(int channel, boolean sustain);
int note_(int note);
int octave_ (int note);
void seq_synth_play_note(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity);
void seq_synth_stop_note(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity);
void seq_synth_init(t_seq_synth *synth);
void seq_synth_loop(t_seq_synth *synth);



void seq_tuning_12(t_seq_synth_tuning *tuning);
void seq_tuning_13(t_seq_synth_tuning *tuning);
void seq_tuning_14(t_seq_synth_tuning *tuning);
void seq_tuning_15(t_seq_synth_tuning *tuning);
void seq_tuning_16(t_seq_synth_tuning *tuning);
void seq_tuning_17(t_seq_synth_tuning *tuning);


void seq_track_delete(t_seq_track *track);
t_seq_track *seq_track_create();
t_seq_sequencer *seq_sequencer_create();
uint64_t  seq_track_update(t_seq_sequencer *sequencer, t_seq_track *track, uint32_t delta);
void IRAM_ATTR onTimer();
void seq_sequencer_setup_timer(t_seq_sequencer *sequencer, uint16_t bpm, uint16_t precision);
uint64_t seq_sequencer_update(t_seq_sequencer *sequencer, uint32_t delta);
void  seq_sequencer_loop(t_seq_sequencer *sequencer);


int8_t seq_sd_init();
void SD_Init(void *parameter);
void seq_sd_init_task(t_seq_sequencer *sequencer);


t_seq_console *seq_console_create();
void seq_console_init(t_seq_console *console);

void seq_register_commands();

#endif
