#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#define CIRCULAR_BUFFER_SIZE

struct circular_buffer {
		char buffer[CIRCULAR_BUFFER_SIZE];
		int head, tail;
}

char cbuffer_pop (struct circular_buffer *b);

void cbuffer_push (struct circular_buffer *b, char data);

int cbuffer_empty (struct circular_buffer *b);

int cbuffer_full (struct circular_buffer *b);

#endif /* CIRCULAR_BUFFER_H */
