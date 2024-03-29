/*
 * entry.S - Entry point to system mode from user mode
 */

#include <asm.h>
#include <segment.h>


/**************************************************/
/**** Save & Restore ******************************/
/**                                              **/
/** When we change to privilege level 0 (kernel) **/
/** (through an interrupt, a system call, an     **/
/** exception ...) we must save the state of the **/
/** currently running task (save).               **/
/**                                              **/
/** Stack layout in 'systemCall':                **/
/**                                              **/
/**   0(%esp) - %edx    \                        **/
/**   4(%esp) - %ecx     |                       **/
/**   8(%esp) - %ebx     |                       **/
/**   C(%esp) - %esi     | Register saved        **/
/**  10(%esp) - %edi     |  by 'save'            **/
/**  14(%esp) - %ebp     |                       **/
/**  18(%esp) - %eax     |                       **/
/**  1C(%esp) - %ds      |                       **/
/**  20(%esp) - %es      |                       **/
/**  24(%esp) - %fs      |                       **/
/**  28(%esp) - %gs     /                        **/
/**  2C(%esp) - %eip    \                        **/
/**  30(%esp) - %cs      |                       **/
/**  34(%esp) - %eflags  |  Return context saved **/
/**  38(%esp) - %oldesp  |   by the processor.   **/
/**  3C(%esp) - %oldss  /                        **/
/**                                              **/
/**************************************************/

#define SAVE_ALL \
      pushl %gs; \
      pushl %fs; \
      pushl %es; \
      pushl %ds; \
      pushl %eax; \
      pushl %ebp; \
      pushl %edi; \
      pushl %esi; \
      pushl %ebx; \
      pushl %ecx; \
      pushl %edx; \
      movl $__KERNEL_DS, %edx; \
      movl %edx, %ds; \
      movl %edx, %es

#define RESTORE_ALL \
      popl %edx; \
      popl %ecx; \
      popl %ebx; \
      popl %esi; \
      popl %edi; \
      popl %ebp; \
      popl %eax; \
      popl %ds; \
      popl %es; \
      popl %fs; \
      popl %gs

#define EOI \
      movb $0x20, %al; \
      outb %al, $0x20

/* interruptions */

ENTRY(keyboard_handler)
	SAVE_ALL
	call update_user_to_system_ticks
     	EOI
	call keyboard_routine
	call update_system_to_user_ticks
	RESTORE_ALL
	iret

ENTRY(clock_handler)
	SAVE_ALL
	call update_user_to_system_ticks
	EOI
	call clock_routine
	call update_system_to_user_ticks
	RESTORE_ALL
	iret

/* exceptions */
        
ENTRY(page_fault_handler_custom)
        SAVE_ALL
        call page_fault_routine_custom
        RESTORE_ALL
        add $4, %esp
        iret

/* syscalls */

ENTRY(system_call_handler)
	SAVE_ALL
	call update_user_to_system_ticks
        cmpl $0, %eax
        jl err_int
        cmpl $MAX_SYSCALL, %eax
        jg err_int
	call *sys_call_table(, %eax, 0x4)
        jmp end_int
err_int:
        movl $-38, %eax
end_int:
        movl %eax, 0x18(%esp)
	RESTORE_ALL
	iret

ENTRY(system_call_handler_fast)
	push $__USER_DS
	push %ebp // User stack address
	pushfl
	push $__USER_CS
	push 4(%ebp) // User return address
	SAVE_ALL
        cmpl $0, %eax
        jl err_sysenter
        cmpl $MAX_SYSCALL, %eax
        jg err_sysenter
	call *sys_call_table(, %eax, 0x4)
        jmp end_sysenter
err_sysenter:
        movl $-38, %eax
end_sysenter:
        movl %eax, 0x18(%esp)
	RESTORE_ALL
	movl (%esp), %edx // Return address
	movl 12(%esp), %ecx // User stack address
	//call update_system_to_user_ticks
	sti // Enable interrupts again
	sysexit
