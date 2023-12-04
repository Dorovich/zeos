#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <list.h>

#define SEM_NUM 10

struct sem_t {
    int count;
    struct list_head blocked;
    struct list_head sem_list;
};

extern struct sem_t semaphores[SEM_NUM];
extern struct list_head sem_freequeue;

void init_semaphores(void);
struct sem_t *semCreate(int initial_value);
int semWait(struct sem_t *s);
int semSignal(struct sem_t *s);
int semDestroy(struct sem_t *s);

#endif /* SEMAPHORE_H */
