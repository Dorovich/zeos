#include <cbuffer.h>
<<<<<<< HEAD
void cbuffer_init(struct cbuffer *b) {
    b->head = b->tail = 0;
}
=======

void cbuffer_init(struct cbuffer *b) {
    b->head = b->tail = 0;
}

>>>>>>> final
char cbuffer_pop(struct cbuffer *b) {
    char data = b->buffer[b->head];
    b->head = (b->head+1)%CBUFFER_SIZE;
    return data;
}
<<<<<<< HEAD
=======

>>>>>>> final
void cbuffer_push(struct cbuffer *b, char data) {
    b->buffer[b->tail] = data;
    b->tail = (b->tail+1)%CBUFFER_SIZE; 
}
<<<<<<< HEAD
=======

>>>>>>> final
int cbuffer_empty(struct cbuffer *b) {
    if (b->head == b->tail) return 1;
    return 0;
}
<<<<<<< HEAD
=======

>>>>>>> final
int cbuffer_full(struct cbuffer *b) {
    if (b->head == (b->tail+1)%CBUFFER_SIZE) return 1;
    return 0;
}
<<<<<<< HEAD
=======

>>>>>>> final
