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
  uint16_t        idle_ticks;
  uint8_t         message[SEQ_CONFIG_MAX_MESSAGE_LENGTH];
}                 t_seq_event;

/**
 * Sequencer track structure
 */
typedef struct    s_seq_track
{
  s_seq_ring      *ring;
  uint16_t        elapsed_ticks;
  uint16_t        idle_ticks;
  uint8_t         enable;
  t_seq_event     *loaded;
}                 t_seq_track;

/**
 * Sequencer structure
 */
typedef struct    s_seq_sequencer
{
  t_seq_track     tracks[SEQ_CONFIG_MAX_TRACKS];
  uint16_t        idle_ticks;
  uint16_t        elapsed_ticks;
  uint8_t         enable;
}                 t_seq_sequencer;

#endif
