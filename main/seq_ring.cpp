#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "seq_include.h"

int seq_ring_init(t_seq_ring *ring, uint32_t element_size,
            uint32_t max_elements)
{
  if (!ring)
    return (-1);
  ring->BUFFER = new cppQueue (element_size, max_elements, FIFO, true);
  return (0);
}

int seq_ring_init2(t_seq_ring *ring, uint32_t element_size,
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
  return (ring->BUFFER->getCount());
  return (0);
  return ((ring->head <= ring->tail)  * (ring->max_elements - ring->tail - ring->head - 1)
          + (ring->head > ring->tail) * (ring->head - ring->tail - 1));
}

uint32_t seq_ring_elements_used2(t_seq_ring *ring)
{
  if (ring->head > ring->tail)
  {
    // En este caso se han escrito algunos caracteres ,es la diferencia entre los dos
    return (ring->head - ring->tail);
  }
  if (ring->head < ring->tail)
  {
    // posiciones invertidas
    return (ring->tail - ring->head);
  }
  return (0);
	return ((ring->head <= ring->tail)	* (ring->max_elements - ring->tail - ring->head - 1)
			    + (ring->head > ring->tail) * (ring->head - ring->tail - 1));
}

uint32_t    seq_ring_bytes_used(t_seq_ring *ring)
{
	return (seq_ring_elements_used(ring) * ring->element_size);
}

uint32_t seq_ring_elements_free(t_seq_ring *ring)
{
  //return (ring->BUFFER->getRemainingCount());
	return ((ring->max_elements - seq_ring_elements_used(ring)) - 1);
}

uint32_t	seq_ring_bytes_free(t_seq_ring *ring)
{
	return (seq_ring_elements_free(ring) * ring->element_size);
}

int seq_ring_push(t_seq_ring *ring, void *src)
{
  ring->BUFFER->push(src);
  return (0);
}

int seq_ring_push2(t_seq_ring *ring, void *src)
{
	uint32_t	next;

	if (seq_ring_elements_free(ring) == 0)
		return (-1);
	next = ring->head + 1;
	if (next >= ring->max_elements)
		next = 0;
	memcpy(ring->buffer + (ring->head * ring->element_size), src,
			ring->element_size);
	ring->head = next;
	return (0);
}

int seq_ring_pop(t_seq_ring *ring, void *dst)
{
  ring->BUFFER->pop(dst);
  return (0);
}

int seq_ring_pop2(t_seq_ring *ring, void *dst)
{
	uint32_t	next;

	if (!seq_ring_elements_used(ring))
		return (-1);
	next = ring->tail + 1;
	if (next >= ring->max_elements)
		next = 0;
	memcpy (dst, ring->buffer + (ring->tail * ring->element_size),
			ring->element_size);
	ring->tail = next;
	return (0);
}
