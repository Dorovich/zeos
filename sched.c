/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry(l, struct task_struct, list);
}

struct list_head freequeue;
struct list_head readyqueue;
extern struct list_head blocked;

struct task_struct *idle_task;

unsigned char pid_list[NR_PIDS];

void writeMSR(int number, int value);
unsigned int get_ebp();
void set_esp(unsigned int new_esp);

/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1);
}

void init_idle (void)
{
	struct list_head *e = list_first(&freequeue);
	list_del(e);
	struct task_struct *t = list_entry(e, struct task_struct, list);
	t->PID = 0;
        pid_list[t->PID] = 1;
	allocate_DIR(t);
	union task_union *u = (union task_union *)t;
        u->stack[KERNEL_STACK_SIZE-2] = 0; // ebp
	u->stack[KERNEL_STACK_SIZE-1] = (unsigned long)cpu_idle; // @ret
	t->kernel_esp = &(u->stack[KERNEL_STACK_SIZE-2]);
	idle_task = t;
}

void init_task1(void)
{
	struct list_head *e = list_first(&freequeue);
	list_del(e);
	struct task_struct *t = list_entry(e, struct task_struct, list);
	t->PID = 1;
        pid_list[t->PID] = 1;
	allocate_DIR(t);
	set_user_pages(t);
	union task_union *u = (union task_union *)t;
	tss.esp0 = (long unsigned int)&(u->stack[KERNEL_STACK_SIZE]);
	writeMSR(0x175, (int)&(u->stack[KERNEL_STACK_SIZE]));
	set_cr3(t->dir_pages_baseAddr);
}


void init_sched()
{
        INIT_LIST_HEAD(&freequeue);
	for(int i=0; i<NR_TASKS; ++i) {
		list_add_tail(&(task[i].task.list), &freequeue);
	}
	INIT_LIST_HEAD(&readyqueue);
}

struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}

void inner_task_switch(union task_union *t) {
	tss.esp0 = (long unsigned int)&(t->stack[KERNEL_STACK_SIZE]);
	writeMSR(0x175, (int)&(t->stack[KERNEL_STACK_SIZE]));
	set_cr3(t->task.dir_pages_baseAddr);
        quantum = get_quantum(t->task);
	current()->kernel_esp = get_ebp();
	set_esp(t->task.kernel_esp);
}

int ret_from_fork() {
    return 0;
}

void update_sched_data_rr (void) {
    --quantum;
}

int needs_sched_rr (void) {
    if (quantum == 0) return 1;
    return 0;
}

void update_process_state_rr (struct task_struct *t, struct list_head *dst_queue) {
    if (current() != t) list_del(t->list);
    if (dst_queue != NULL) list_add_tail(&(t->list), dst_queue);
}

void sched_next_rr (void) {
    if (list_empty(&readyqueue)) {
        task_switch(idle_task);
    } else {
        struct list_head *e = list_first(&readyqueue);
        list_del(e);
        struct task_struct *t = list_entry(e, struct task_struct, list);
        task_switch(t);
    }
}

void schedule (void) {
    update_sched_data_rr();
    if (needs_sched_rr()) {
        if (current() != idle_task)
            update_process_state_rr(current(), &readyqueue);
        sched_next_rr();
    }
}

int get_quantum (struct task_struct *t) {
    return t->quantum;
}

void set_quantum (struct task_struct *t, int new_quantum) {
    t->quantum = new_quantum;
}
