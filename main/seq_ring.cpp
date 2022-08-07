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

int seq_ring_init(t_seq_ring *ring, uint32_t element_size,
						uint32_t max_elements)
{
	if (!ring)
		return (-1);
	ring->buffer = (uint8_t *)malloc(max_elements * element_size);
	if (!ring->buffer)
		return (-1);
	ring->element_size = element_size;
	ring->max_elements = max_elements;
	ring->head = 0;
	ring->tail = 0;
	return (0);
}

uint32_t seq_ring_elements_used(t_seq_ring *ring)
{
	return ((ring->head - ring->tail < 0)
			* (ring->max_elements + ring->head - ring->tail - 1)
			+ (ring->head - ring->tail >= 0) * (ring->head - ring->tail));
}

uint32_t    seq_ring_bytes_used(t_seq_ring *ring)
{
	return (seq_ring_elements_used(ring) * ring->element_size);
}

uint32_t seq_ring_elements_free(t_seq_ring *ring)
{
	return (ring->max_elements - seq_ring_elements_used(ring) - 1);
}

uint32_t	seq_ring_bytes_free(t_seq_ring *ring)
{
	return (seq_ring_elements_free(ring) * ring->element_size);
}

int seq_ring_push(t_seq_ring *ring, void *src)
{
	uint32_t	next;

	if (!seq_ring_elements_free(ring))
		return (-1);
	next = ring->head + 1;
	if (next == ring->max_elements)
		next = 0;
	memcpy(ring->buffer + (ring->head * ring->element_size), src,
			ring->element_size);
	ring->head = next;
	return (0);
}

int seq_ring_pop(t_seq_ring *ring, void *dst)
{
	uint32_t	next;

	if (!seq_ring_elements_used(ring))
		return (-1);
	next = ring->tail + 1;
	if (next > ring->max_elements)
		next = 0;
	memcpy (dst, ring->buffer + (ring->tail * ring->element_size),
			ring->element_size);
	ring->tail = next;
	return (0);
}