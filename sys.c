/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>
#include <utils.h>
#include <io.h>
#include <mm.h>
#include <mm_address.h>
#include <sched.h>
#include <stats.h>
#include <interrupt.h>
#include <cbuffer.h>

#define LECTURA 0
#define ESCRIPTURA 1

#define WRITE_MAX 256

extern unsigned char pid_list[NR_PIDS];

int check_fd(int fd, int permissions)
{
    if (fd!=1) return -9; /*EBADF*/
    if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
    return 0;
}

int sys_ni_syscall()
{
    return -38; /*ENOSYS*/
}

int sys_getpid()
{
    return current()->PID;
}

int sys_fork()
{
    // pillar un PCB libre
    struct list_head *e = list_first(&freequeue);
    if (list_empty(e)) return -1;
    list_del(e);
    struct task_struct *t = list_entry(e, struct task_struct, list);

    // copiar el task_union del padre al hijo
    copy_data(current(), t, sizeof(union task_union));
    allocate_DIR(t);

    page_table_entry *child_PT = get_PT(t), *parent_PT = get_PT(current());
    int new_frame, pag;

    // copiar paginas del kernel
    for (pag=0; pag<NUM_PAG_KERNEL; pag++) {
        child_PT[(KERNEL_START>>12)+pag] = parent_PT[(KERNEL_START>>12)+pag];
    }

    // copiar paginas del codigo
    for (pag=0; pag<NUM_PAG_CODE; pag++) {
        child_PT[PAG_LOG_INIT_CODE+pag] = parent_PT[PAG_LOG_INIT_CODE+pag];      
    }

    // buscar entrada libre de la TP
    int temp_entry=-1;
    pag=TOTAL_PAGES-1;
    while (pag>(KERNEL_START>>12)+NUM_PAG_KERNEL && temp_entry<0) {
        if (parent_PT[pag].entry == 0) temp_entry = pag;
        --pag;
    }
    if (temp_entry<0) return -1;
  
    // alojar paginas de datos para el hijo y copiar los del padre
    for (pag=0; pag<NUM_PAG_DATA; pag++) {
        new_frame = alloc_frame();
        if (new_frame < 0) return -1;
      
        set_ss_pag(child_PT, PAG_LOG_INIT_DATA+pag, new_frame);
        set_ss_pag(parent_PT, temp_entry, new_frame);
      
        void *parent_page = (void *)((PAG_LOG_INIT_DATA+pag)<<12);
        void *child_page = (void *)(temp_entry<<12);
        copy_data(parent_page, child_page, PAGE_SIZE);
      
        set_cr3(current()->dir_pages_baseAddr); // flush TLB
    }

    del_ss_pag(parent_PT, temp_entry);
    set_cr3(current()->dir_pages_baseAddr);
  
    // cambiar campos del task_struct del hijo no comunes con el padre
  
    // asignar un PID al proceso
    int new_pid;
    do {
        new_pid = (zeos_ticks+zeos_ticks+zeos_ticks)%NR_PIDS;
    } while (pid_list[new_pid] == 1);
    t->PID = new_pid;
    pid_list[new_pid] = 1;

    // inicializar sus stats a cero
    INIT_STATS(&t->stats);
  
    // preparar la pila del hijo
    union task_union *u = (union task_union *)t;
    // unsigned int pos = KERNEL_STACK_SIZE-1 - (5+11+1); /* ctx HW + ctx SW + @ret al handler */
    //int pos = KERNEL_STACK_SIZE-18; /* ctx HW + ctx SW + @ret al handler = 17 */
    int offset = 18; //17?
    u->stack[KERNEL_STACK_SIZE-offset-1] = 0; // ebp
    u->stack[KERNEL_STACK_SIZE-offset] = (unsigned long)ret_from_fork; // @ret
    t->kernel_esp = (unsigned long int)&u->stack[KERNEL_STACK_SIZE-offset-1];
    
    // insertar hijo en readyqueue
    list_add_tail(&t->list, &readyqueue);
    
    return new_pid;
}

void sys_exit()
{
    int pag;
    page_table_entry *current_PT = get_PT(current());

    for (pag=0; pag<NUM_PAG_DATA; pag++) {
        free_frame(get_frame(current_PT, PAG_LOG_INIT_DATA+pag));
    }

    pid_list[current()->PID] = 0;
    update_process_state_rr(current(), &freequeue);
    sched_next_rr();
}

int sys_write(int fd, char *buffer, int size)
{
    // comprovaciones
    int valido = check_fd(fd, ESCRIPTURA);
    if (valido < 0) return valido;
    if (buffer == NULL) return -1;
    if (size < 0) return -1;

    // copiar datos del usuario y escribir
    char sys_buffer[WRITE_MAX];
    int s = 0, written = 0, returned, to_write;
    while (s < size) {
        if (size-s <= WRITE_MAX) to_write = size-s;
        else to_write = WRITE_MAX;
        
        copy_data(&buffer[s], sys_buffer, to_write);
        s += to_write;
        
        switch (fd) {
        case 1:
            returned = sys_write_console(sys_buffer, to_write);
            break;
        }

        if (returned < 0) return returned;
        else written += returned;
    }

    return written;
}

int sys_gettime()
{
    return zeos_ticks;
}

int sys_get_stats (int pid, struct stats *st)
{
    struct task_struct *p = get_process_by_pid(pid);
    if (p == NULL) return -1;
    if (st == NULL) return -1;
    copy_data(&p->stats, st, sizeof(struct stats));
    
    return 0;
}

int sys_waitKey(char *b, int timeout) {
    if (b == NULL) return -1;
    if (!cbuffer_empty(&keyboard_buffer)) {
        current()->keyboard_read = cbuffer_pop(&keyboard_buffer);
        copy_to_user(&(current()->keyboard_read), b, sizeof(char));
        return 0;
    } else {
        // pasar de segundos a ticks (1 s -> 18 ticks)
        current()->timeout = timeout*18;
        update_process_state_rr(current(), &keyboard_blocked);
        sched_next_rr();

        if (current()->timeout > 0) {
            copy_to_user(&(current()->keyboard_read), b, sizeof(char));
            return 0;
        }
    }
    return -1;
}

int sys_gotoXY (int x, int y) {
    return point_to(x, y, point.fg, point.bg);
}

int sys_changeColor (int fg, int bg) {
    return point_to(point.x, point.y, fg, bg);
}

int sys_clrscr (char *b) {
    return set_screen(b);
}

int sys_threadCreateWithStack(void (*function)(void *arg), int N, void *parameter) {
    // pillar un TCB (PCB) libre
    if (list_empty(&freequeue)) return -1;
    struct list_head *e = list_first(&freequeue);
    struct task_struct *t = list_entry(e, struct task_struct, list);
    
    // copiar el task_union
    copy_data(current(), t, sizeof(union task_union));

    // TODO: alojar la pila para el nuevo thread
  
    // inicializar sus stats a cero (nose si de tiene que hacer)
    INIT_STATS(&t->stats);
  
    // TODO: preparar la pila
    union task_union *u = (union task_union *)t;
    int stack_offset = 18;
    u->stack[KERNEL_STACK_SIZE-stack_offset-1] = 0; // ebp
    u->stack[KERNEL_STACK_SIZE-stack_offset] = (unsigned long)ret_from_fork; // @ret
    t->kernel_esp = (unsigned long int)&u->stack[KERNEL_STACK_SIZE-stack_offset-1];
    
    // insertar en readyqueue
    update_process_state_rr(t, &readyqueue);
    
    return 0;
}
