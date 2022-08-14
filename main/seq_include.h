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

#ifndef SEQ_INCLUDE_H
# define SEQ_INCLUDE_H

# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include "seq_configuration.h"
# include "seq_structures.h"
# include "seq_peripheral.h"

/**
 * seq_st7920.cpp prototypes
 */
void        seq_st7920_prepare(void);
void        seq_st7920_init(void);
//void        seq_st7920_draw(void (*draw_callback)(SEQ_U8G2_CLASS));

/**
 * seq_mpr121.cpp prototypes
 */
void        seq_mpr121_init(void);
void        seq_mpr121_get_events(void);

/**
 * seq_ring.cpp prototypes
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
 * seq_mpr121.cpp prototypes
 */
void        seq_keyboard_init(t_seq_keyboard *keyboard);
void        seq_keyboard_device_touched(Adafruit_MPR121 *seq_mpr121, uint16_t *last_touched, uint8_t key_offset);
void        seq_keyboard_instrument_press(uint8_t key);
void        seq_keyboard_instrument_release(uint8_t key);
void        seq_keyboard_set_instrument_mode(t_seq_keyboard *keyboard);
void        seq_keyboard_touched(t_seq_keyboard *keyboard);
void        seq_keyboard_text_press(uint8_t key);
void        seq_keyboard_text_release(uint8_t key);
void        seq_keyboard_set_text_mode(t_seq_keyboard *keyboard);



void seq_ym2413_shift (uint8_t value);
void seq_synth_reset_channels(t_seq_synth *synth);
int8_t  seq_synth_get_channel(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity);
int8_t  seq_synth_release_channel(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity);
void seq_ym2413_setup_pins();
void seq_ym2413_select_chip();
void seq_ym2413_reset_chip();
void seq_ym2413_setup();
void seq_ym2413_write(byte address, byte data);
void seq_ym2413_play_note(int channel, int pitch, int oct, int instrument, int vol);
void seq_ym2413_stop_note(int channel, boolean sustain);
int note_(int note);
int octave_ (int note);
void seq_synth_play_note(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity);
void seq_synth_stop_note(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity);
void seq_synth_init(t_seq_synth *synth);
void seq_synth_loop(t_seq_synth *synth);




















#endif
