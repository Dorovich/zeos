#include <semaphore.h>
#include <sched.h>
#include <list.h>

struct sem_t semaphores[SEM_NUM];
struct list_head sem_freequeue;

void init_semaphores (void) {
    INIT_LIST_HEAD(&sem_freequeue);
    for (int i=0; i<SEM_NUM; ++i)
        list_add_tail(&(semaphores[i].sem_list), &sem_freequeue);
}

struct sem_t *semCreate (int initial_value) {
    if (list_empty(&sem_freequeue)) return NULL;
    struct list_head *e = list_first(&sem_freequeue);
    list_del(e);
    struct sem_t *new_sem = list_entry(e, struct sem_t, sem_list);
    new_sem->count = initial_value;
    INIT_LIST_HEAD(&new_sem->blocked);
    return new_sem;
}

int semWait (struct sem_t *s) {
    if (s == NULL) return -1;
    s->count--;
    if (s->count < 0) {
        update_process_state_rr(current(), &s->blocked);
        sched_next_rr();
    }
    return 0;
}

int semSignal (struct sem_t *s) {
    if (s == NULL) return -1;
    s->count++;
    if (s->count <= 0 && !list_empty(&s->blocked)) {
        struct list_head *e = list_first(&s->blocked);
        struct task_struct *t = list_entry(e, struct task_struct, list);
        update_process_state_rr(t, &readyqueue);
    }
    return 0;
}

int semDestroy (struct sem_t *s) {
    if (s == NULL) return -1;
    
    /*
      hay que escoger si los threads que estaban bloqueados se matan directamente
      o se ponen en la readyqueue, depende de lo q queramos hacer.
      
      idea: poner una variable nueva en el pcb, 'called_to_die' (por defecto 0),
      que se compruebe en el semWait despues del sched_next_rr y que si es 1 la
      syscall devuelva -1. cuando se hace un semDestroy los q estaban bloqueados
      en ese semaforo se debloquean con called_to_die=1.
    */
    
    list_add_tail(&s->sem_list, &sem_freequeue);
    return 0;
}
