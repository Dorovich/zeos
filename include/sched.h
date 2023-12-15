/*
 * sched.h - Estructures i macros pel tractament de processos
 */

#ifndef __SCHED_H__
#define __SCHED_H__

#include <list.h>
#include <types.h>
#include <mm_address.h>
#include <stats.h>

#define NR_TASKS      10
#define KERNEL_STACK_SIZE	1024

#define NR_PIDS 256

#define SEM_NUM 10

extern struct list_head freequeue;
extern struct list_head readyqueue;

extern unsigned int global_PID;
extern unsigned int global_TID;

/* enum state_t { ST_RUN, ST_READY, ST_BLOCKED }; */

struct task_struct {
    int PID;			/* Process ID. This MUST be the first field of the struct. */
    int TID;
    struct list_head list;
    unsigned long kernel_esp;
    page_table_entry *dir_pages_baseAddr;
    int quantum;
    struct stats stats;
    int timeout;
    char keyboard_read;
    int called_to_die;
    int temp_stack_page;
    int temp_stack_size;
    struct list_head mem_list;
};

union task_union {
    struct task_struct task;
    unsigned long stack[KERNEL_STACK_SIZE];    /* pila de sistema, per procés */
};

extern union task_union task[NR_TASKS]; /* Vector de tasques */

extern int quantum;

struct sem_t {
    int count;
    struct list_head blocked;
    struct list_head sem_list;
};

extern struct sem_t semaphores[SEM_NUM];
extern struct list_head sem_freequeue;

#define KERNEL_ESP(t)       	(DWord) &(t)->stack[KERNEL_STACK_SIZE]

#define INITIAL_ESP       	KERNEL_ESP(&task[1])

/* Inicialitza les dades del proces inicial */
void init_task1(void);

void init_idle(void);

void init_sched(void);

struct task_struct *current();

void task_switch(union task_union*t);

void inner_task_switch(union task_union*t);

struct task_struct *list_head_to_task_struct(struct list_head *l);

int allocate_DIR(struct task_struct *t);

page_table_entry *get_PT (struct task_struct *t) ;

page_table_entry *get_DIR (struct task_struct *t) ;

int ret_from_fork ();

struct task_struct* get_process_by_pid (int pid);

/* Headers for the scheduling policy */
void sched_next_rr();
void update_process_state_rr(struct task_struct *t, struct list_head *dest);
int needs_sched_rr();
void update_sched_data_rr();
void schedule();

int get_quantum (struct task_struct *t);
void set_quantum (struct task_struct *t, int new_quantum);

void init_semaphores(void);

#endif  /* __SCHED_H__ */
