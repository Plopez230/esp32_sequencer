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
uint32_t seq_ring_size(t_seq_ring *ring);
uint32_t seq_ring_bytes_used(t_seq_ring *ring);
uint32_t seq_ring_bytes_free(t_seq_ring *ring);
int seq_ring_push(t_seq_ring *ring, uint8_t *data_src);
int seq_ring_pushn(t_seq_ring *ring, void *data_src, uint32_t n);
int seq_ring_pop(t_seq_ring *ring, uint8_t *data_dst);
int seq_ring_popn(t_seq_ring *ring, void *data_dst, uint32_t n);
int seq_ring_init(t_seq_ring *ring, uint32_t size_in_bytes);

#endif
