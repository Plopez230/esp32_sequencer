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

#include "seq_include.h"

t_seq_synth *seq_synth_create()
{
  t_seq_synth *new_synth;

  new_synth = (t_seq_synth *)malloc(sizeof(t_seq_synth));
  if (!new_synth)
    return (NULL);
  new_synth->tuning = (s_seq_synth_tuning *)malloc(sizeof(s_seq_synth_tuning));
  if (!new_synth->tuning)
  {
    free(new_synth);
    return (NULL);
  }
  new_synth->input_buffer = new cppQueue(3, SEQ_CONFIG_SYNTH_BUFFER_SIZE, FIFO, true);
  if (!new_synth->input_buffer)
  {
    free(new_synth);
    return (NULL);
  }
  return (new_synth);
}

void seq_synth_reset_channels(t_seq_synth *synth)
{
  uint8_t channel_counter;

  channel_counter = 0;
  while (channel_counter < 8)
  {
    synth->channels[channel_counter].instrument = 0;
    synth->channels[channel_counter].note = 0;
    synth->channels[channel_counter].velocity = 0;
    synth->channels[channel_counter].used = 0;
    channel_counter++;
  }
}


int8_t  seq_synth_get_channel2(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity)
{
  uint8_t channel;

  channel = synth->last_active_channel + 1;
  if (channel > 8) channel = 0;
  synth->channels[channel].instrument = instrument;
  synth->channels[channel].note = note;
  synth->channels[channel].velocity = velocity;
  synth->channels[channel].used = 1;
  synth->last_active_channel = channel;
  return (channel);
}

int8_t  seq_synth_get_channel(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity)
{
  int8_t channel_counter;

  channel_counter = 0;
  while (channel_counter < 9)
  {
    if (!synth->channels[channel_counter].used)
    {
      synth->channels[channel_counter].instrument = instrument;
      synth->channels[channel_counter].note = note;
      synth->channels[channel_counter].velocity = velocity;
      synth->channels[channel_counter].used = 1;
      return (channel_counter);
    }
    channel_counter++;
  }
  synth->channels[0].instrument = instrument;
  synth->channels[0].note = note;
  synth->channels[0].velocity = velocity;
  synth->channels[0].used = 1;
  return (0);
}

int8_t  seq_synth_release_channel(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity)
{
  int x = 0;

  while (x < 9){
    if (synth->channels[x].used && synth->channels[x].note == note && synth->channels[x].instrument == instrument) {
      synth->channels[x].used = false;
      return x;
    }
    x++;
  }
  return -1;
}

int note_(int note) {
  return (int) note % 12;
}

int octave_ (int note) {
  return (int) note / 12;
}

void seq_synth_play_note(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity)
{
  uint8_t channel = seq_synth_get_channel(synth, instrument, note, velocity);
  seq_ym2413_play_note(synth->tuning, channel, note, instrument, 0);
}

void seq_synth_stop_note(t_seq_synth *synth, uint8_t instrument, uint8_t note, uint8_t velocity)
{
  uint8_t channel = seq_synth_release_channel(synth, instrument, note, velocity);
  if (channel == -1)
    return;
  seq_ym2413_stop_note(channel, false);
}

void seq_synth_init(t_seq_synth *synth)
{
  seq_ym2413_setup();
  seq_synth_reset_channels(synth);
  seq_tuning_12(synth->tuning);
}

void seq_synth_loop(t_seq_synth *synth)
{
  char    event[3];
  uint8_t command;
  uint8_t channel;
  uint8_t velocity;
  
  if (!synth->input_buffer)
    return;
  while (!synth->input_buffer->isEmpty())
  {
    synth->input_buffer->pop(event);
    command = event[0] & 0xf0;
    channel = event[0] & 0x0f;
    velocity = (event[2] * 16) >> 4;
    if (command == 144) {
      seq_synth_play_note(synth, channel, event[1], velocity);
    }
    if (command == 128) {
      seq_synth_stop_note(synth, channel, event[1], velocity);
    }
  }
}
