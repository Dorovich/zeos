#ifndef CBUFFER_H
#define CBUFFER_H
<<<<<<< HEAD
=======

>>>>>>> final
#define CBUFFER_SIZE 10

struct cbuffer {
    char buffer[CBUFFER_SIZE];
    int head, tail;
};

// -----------------------------
// |   | A | Z | F |   |   |   |
// -----------------------------
//     ^head       ^tail

void cbuffer_init (struct cbuffer *b);
<<<<<<< HEAD
char cbuffer_pop (struct cbuffer *b);
void cbuffer_push (struct cbuffer *b, char data);
int cbuffer_empty (struct cbuffer *b);
=======

char cbuffer_pop (struct cbuffer *b);

void cbuffer_push (struct cbuffer *b, char data);

int cbuffer_empty (struct cbuffer *b);

>>>>>>> final
int cbuffer_full (struct cbuffer *b);

#endif /* CBUFFER_H */
