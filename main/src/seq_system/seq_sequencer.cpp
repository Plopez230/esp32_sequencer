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

#include "../seq.h"


portMUX_TYPE seq_sequencer_timer_mux = portMUX_INITIALIZER_UNLOCKED;
uint64_t  seq_sequencer_time;

void seq_track_delete(t_seq_track *track)
{
  if (track)
  {
    delete track->track_buffer;
    //free (track);
  }
}


t_seq_track *seq_track_create()
{
  t_seq_track *new_track;

  new_track = (t_seq_track *)malloc(sizeof(t_seq_track));
  if (!new_track)
    return (NULL);
  new_track->track_buffer = new cppQueue(7, SEQ_CONFIG_TRACK_BUFFER_SIZE, FIFO, true);
  new_track->enable = 0;
  return (new_track);
}


t_seq_sequencer *seq_sequencer_create()
{
  t_seq_sequencer *new_sequencer;
  uint8_t         track_idx;

  new_sequencer = (t_seq_sequencer *)malloc(sizeof(t_seq_sequencer));
  if (!new_sequencer)
    return (NULL);
  track_idx = 0;
  while (track_idx < SEQ_CONFIG_MAX_TRACKS)
  {
    new_sequencer->tracks[track_idx] = seq_track_create();
    track_idx++;
  }
  return (new_sequencer);
}

/**
   Update sequencer track.
   adds delta to elapsed ticks, checks if the next event is ready and sends it if true
   returns the idle time to the next event.
*/
uint64_t  seq_track_update(t_seq_sequencer *sequencer, t_seq_track *track, uint32_t delta)
{
  track->elapsed_ticks += delta;
  while (track->next_event.idle_ticks < track->elapsed_ticks)
  {
    // TODO: replace ring with cppQueue
    //seq_ring_push(sequencer->output_buffer, track->next_event.message);
    track->elapsed_ticks -= track->next_event.idle_ticks;
    if (!track->track_buffer->isEmpty()) {
      track->track_buffer->pop(&(track->next_event));
      track->next_event.idle_ticks /= SEQ_CONFIG_PRECISION_DIVIDER;
    }
  }
  return (track->next_event.idle_ticks - track->elapsed_ticks);
}

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&seq_sequencer_timer_mux);
  seq_sequencer_time++;
  portEXIT_CRITICAL_ISR(&seq_sequencer_timer_mux);
}

void seq_sequencer_setup_timer(t_seq_sequencer *sequencer, uint16_t bpm, uint16_t precision)
{
  int timer_id = 0;
  uint64_t prescaler = 80;
  uint64_t counter = 80000000 / (precision * prescaler) * 60 / bpm;

  sequencer->timer = timerBegin(timer_id, prescaler, true);
  timerAttachInterrupt(sequencer->timer, onTimer, true);
  timerAlarmWrite(sequencer->timer, counter, true);
  timerAlarmEnable(sequencer->timer);
}

uint64_t seq_sequencer_update(t_seq_sequencer *sequencer, uint32_t delta)
{
  uint64_t    min_wait;
  uint8_t     track_idx;
  uint64_t    current_wait;
  t_seq_track *current_track;

  min_wait = UINT64_MAX;
  track_idx = 0;
  sequencer->elapsed_ticks += delta;
  if (sequencer->elapsed_ticks >= sequencer->idle_ticks)
  {
    while (track_idx < SEQ_CONFIG_MAX_TRACKS)
    {
      current_track = sequencer->tracks[track_idx];
      if (current_track->enable)
      {
        current_wait = seq_track_update(sequencer, sequencer->tracks[track_idx], sequencer->elapsed_ticks);
        if (current_wait < min_wait)
          min_wait = current_wait;
      }
      track_idx++;
    }
    sequencer->elapsed_ticks = 0;
    sequencer->idle_ticks = min_wait;
  }
  return (min_wait);
}

void  seq_sequencer_loop(t_seq_sequencer *sequencer)
{
  uint64_t  delta_ticks;
  portENTER_CRITICAL_ISR(&seq_sequencer_timer_mux);
  delta_ticks = seq_sequencer_time;
  seq_sequencer_time = 0;
  portEXIT_CRITICAL_ISR(&seq_sequencer_timer_mux);
  seq_sequencer_update(sequencer, delta_ticks);
}
