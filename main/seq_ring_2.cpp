#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct    s_seq_ring
{
  uint8_t         *buffer;
  uint32_t        head;
  uint32_t        tail;
  uint32_t        max_elements;
  uint32_t        element_size;
}                 t_seq_ring;

int			seq_ring_init(t_seq_ring *ring, uint32_t element_size, 
						uint32_t max_elements);
uint32_t    seq_ring_elements_used(t_seq_ring *ring);
uint32_t    seq_ring_bytes_used(t_seq_ring *ring);
uint32_t	seq_ring_elements_free(t_seq_ring *ring);
uint32_t	seq_ring_bytes_free(t_seq_ring *ring);
int			seq_ring_push(t_seq_ring *ring, void *src);
int			seq_ring_pop(t_seq_ring *ring, void *dst);

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

int seq_ring_test(void)
{
	t_seq_ring ring;
	char c;
	seq_ring_init(&ring, 1, 4);
	c = '1';
	seq_ring_push(&ring, &c);
	c = '2';
	seq_ring_push(&ring, &c);
	c = '3';
	seq_ring_push(&ring, &c);
	c = '4';
	seq_ring_push(&ring, &c);
	while (seq_ring_elements_used(&ring))
	{
		seq_ring_pop(&ring, &c);
		printf("%c", c);
	}
	seq_ring_init(&ring, 1, 4);
	c = '1';
	seq_ring_push(&ring, &c);
	c = '2';
	seq_ring_push(&ring, &c);
	c = '3';
	seq_ring_push(&ring, &c);
	c = '4';
	seq_ring_push(&ring, &c);
	while (seq_ring_elements_used(&ring))
	{
		seq_ring_pop(&ring, &c);
		printf("%c", c);
	}
	while (seq_ring_elements_used(&ring))
	{
		seq_ring_pop(&ring, &c);
		printf("%c", c);
	}
	return (0);
}

int main(void)
{
	seq_ring_test();
}