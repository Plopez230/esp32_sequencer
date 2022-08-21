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

#ifndef SEQ_STRUCTURES_H
# define SEQ_STRUCTURES_H



/**
 * Ring buffer structure
 */
typedef struct    s_seq_ring
{
  cppQueue        *BUFFER;
  uint8_t         *buffer;
  uint32_t        head;
  uint32_t        tail;
  uint32_t        max_elements;
  uint32_t        element_size;
}                 t_seq_ring;

/**
 * Event structure
 */
typedef struct    s_seq_event
{
  uint32_t        idle_ticks;
  uint8_t         message[SEQ_CONFIG_MAX_MESSAGE_LENGTH];
}                 t_seq_event;

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
 * Keyboard structure
 */
typedef struct    s_seq_keyboard
{
  Adafruit_MPR121 device_a;
  Adafruit_MPR121 device_c;
  Adafruit_MPR121 device_d;
  cppQueue        *output_buffer;
  uint16_t        last_touched_a;
  uint16_t        last_touched_c;
  uint16_t        last_touched_d;
  uint8_t         operation_mode;
  uint8_t         button_state;
  uint8_t         last_button_state;
  uint32_t        last_debounce_time;
  uint8_t         velocity;
  uint8_t         traspose;
  uint8_t         num_mode;
  uint8_t         mayus_mode;
  uint8_t         midi_channel;
  void            (*press_callback)(s_seq_keyboard *, uint8_t);
  void            (*release_callback)(s_seq_keyboard *, uint8_t);
}                 t_seq_keyboard;

/**
 * Synth tuning structure
 */
typedef struct        s_seq_synth_tuning
{
  uint16_t            f_numbers[32];
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

typedef struct        s_seq_synth
{
  t_seq_synth_channel channels[9];
  cppQueue            *input_buffer;
  t_seq_synth_tuning  *tuning;
  uint8_t             last_active_channel;
}                     t_seq_synth;

typedef struct        s_seq_console
{
  cppQueue            *input_buffer;
  TaskHandle_t        task_handler;
  char                *command_buffer;
  char                *return_buffer;
}                     t_seq_console;

typedef struct        s_seq_system
{
  t_seq_keyboard      *keyboard;
  t_seq_synth         *synth;
  t_seq_sequencer     *sequencer;
  t_seq_console       *console;
}                     t_seq_system;

#endif
