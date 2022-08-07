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

typedef struct    s_seq_track
{
  s_seq_ring      *ring;
  uint16_t        elapsed_ticks;
  uint16_t        idle_ticks;
  uint8_t         enable;
  s_seq_event     *next_event;
}                 t_seq_track;

uint32_t  seq_track_update(t_seq_track *track, uint32_t delta)
{
  track->elapsed_ticks += delta;
  while (track->enable && track->next_event->idle_ticks < track->elapsed_ticks)
  {
    //envio next_event
    track->elapsed_ticks -= track->next_event->idle_ticks;
    //cargo nuevo evento
  }
  return (track->next_event->idle_ticks - track->elapsed_ticks);
}

uint32_t seq_sequencer_update(t_seq_sequencer *sequencer, uint32_t delta)
{
  uint32_t min_idle_ticks;
  uint8_t enable;
  uint8_t track_counter;

  enable = 0;
  track_counter = 0;
  while (track_counter < SEQ_CONFIG_MAX_TRACKS)
  {

    track_counter++;
  }
  
}