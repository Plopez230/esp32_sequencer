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

# include <stdint.h>
# include <stdlib.h>
# include <string.h>

typedef struct    s_seq_ring
{
  uint8_t         *buffer;
  uint32_t        head;
  uint32_t        tail;
  uint32_t        max_elements;
  uint32_t        element_size;
}                 t_seq_ring;

/**
 * Returns the size of the buffer
 */
uint32_t seq_ring_size(t_seq_ring *ring)
{
  if (ring->buffer)
    return (ring->size);
  return (0);
}

/**
 * Returns the number of bytes available to read
 */
uint32_t seq_ring_bytes_used(t_seq_ring *ring)
{
  int dif;

  dif = ring->head - ring->tail;
  dif = (dif < 0) * (ring->size + dif - 1) + (dif >= 0) * (dif);
  return (dif);
}

/**
 * Returns the number of bytes available to write
 */
uint32_t seq_ring_bytes_free(t_seq_ring *ring)
{
  int dif;

  dif = ring->size - seq_ring_bytes_used(ring) - 1;
  return (dif);
}

/**
 * Inserts one byte in the buffer.
 * If there is not space available the data is discarded.
 */
int seq_ring_push(t_seq_ring *ring, uint8_t *data_src)
{
  int next;

  next = ring->head + 1;
  if (next == ring->size)
    next = 0;
  if (!seq_ring_bytes_free(ring))
    return (-1);
  ring->buffer[ring->head] = *data_src;
  ring->head = next;
  return (0);
}

int seq_ring_pushn(t_seq_ring *ring, void *data_src, uint32_t n)
{
  uint8_t *data_pointer;

  if (seq_ring_bytes_free(ring) < n)
    return (-1);
  data_pointer = (uint8_t *)data_src;
  while (n--)
    seq_ring_push(ring, data_pointer++);
  return (0);
}

/**
 * Reads one byte from the buffer
 * If there is not enough data to read returns -1
 * if the read is done correctly returns 0
 */
int seq_ring_pop(t_seq_ring *ring, uint8_t *data_dst)
{
  int next;

  if (!seq_ring_bytes_used(ring))
    return (-1);
  next = ring->tail + 1;
  if (next >= ring->size)
    next = ring->size - next;
  *data_dst = ring->buffer[ring->tail];
  ring->tail = next;
  return (0);
}

int seq_ring_popn(t_seq_ring *ring, void *data_dst, uint32_t n)
{
  uint8_t *data_pointer;

  if (seq_ring_bytes_used(ring) < n)
    return (-1);
  data_pointer = (uint8_t *)data_dst;
  while (n--)
    seq_ring_pop(ring, data_pointer++);
  return (0);
}
/**
 * Initialize the ring with the desired size
 */
int seq_ring_init(t_seq_ring *ring, uint32_t size_in_bytes)
{
  if (ring)
  {
    ring->buffer = (uint8_t *)malloc(size_in_bytes + 1);
    if (ring->buffer)
    {
      ring->size = size_in_bytes;
      ring->head = 0;
      ring->tail = 0;
      return (0);
    }
  }
  return (-1);
}
