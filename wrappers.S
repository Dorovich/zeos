#include <asm.h>

ENTRY(syscall_sysenter)
	push %ecx
	push %edx
	push $SYSENTER_RETURN
	push %ebp
	mov %esp, %ebp
	sysenter
        
ENTRY(SYSENTER_RETURN)
	pop %ebp
	pop %edx
	pop %edx
	pop %ecx
	ret

nok:
	neg %eax
	mov %eax, errno
	mov $-1, %eax
	popl %ebp
	ret

ENTRY(write)
        pushl %ebp
        movl %esp, %ebp
        pushl %ebx

        // paso de parametros
        movl 8(%ebp), %edx
        movl 12(%ebp), %ecx
        movl 16(%ebp), %ebx

        // excepcion
        movl $4, %eax
	call syscall_sysenter
	popl %ebx
	test %eax, %eax
	js nok
	popl %ebp
        ret

ENTRY(gettime)
	pushl %ebp
	movl %esp, %ebp
	movl $10, %eax
	call syscall_sysenter
	popl %ebp
	ret

ENTRY(getpid)
	pushl %ebp
	movl %esp, %ebp
	movl $20, %eax
	call syscall_sysenter
	popl %ebp
	ret
        
ENTRY(fork)
	pushl %ebp
	movl %esp, %ebp
	movl $2, %eax
	call syscall_sysenter
	test %eax, %eax
	js nok	// if (eax < 0) -->
	popl %ebp
	ret

ENTRY(exit)
	pushl %ebp
	movl %esp, %ebp
	movl $1, %eax
	call syscall_sysenter
	popl %ebp
	ret

ENTRY(get_stats)
	pushl %ebp
	movl %esp, %ebp
	movl $35, %eax
	movl 8(%ebp), %edx;	//pid
	movl 12(%ebp), %ecx;	//st
	call syscall_sysenter
	test %eax, %eax
	js nok	// if (eax < 0) -->
	popl %ebp
	ret

ENTRY(waitKey)
	pushl %ebp
	movl %esp, %ebp
	movl $11, %eax
	movl 8(%ebp), %edx;	//b*
	movl 12(%ebp), %ecx;	//timeout
	call syscall_sysenter
	test %eax, %eax
	js nok	// if (eax < 0) -->
	popl %ebp
	ret
        
ENTRY(gotoXY)
	pushl %ebp
	movl %esp, %ebp
	movl $12, %eax
	movl 8(%ebp), %edx;	//x
	movl 12(%ebp), %ecx;	//y
	call syscall_sysenter
	test %eax, %eax
	js nok	// if (eax < 0) -->
	popl %ebp
	ret

ENTRY(changeColor)
	pushl %ebp
	movl %esp, %ebp
	movl $13, %eax
	movl 8(%ebp), %edx;	//fg
	movl 12(%ebp), %ecx;	//bg
	call syscall_sysenter
	test %eax, %eax
	js nok	// if (eax < 0) -->
	popl %ebp
	ret

ENTRY(clrscr)
	pushl %ebp
	movl %esp, %ebp
	movl $14, %eax
	movl 8(%ebp), %edx;	//b
	call syscall_sysenter
	test %eax, %eax
	js nok	// if (eax < 0) -->
	popl %ebp
	ret

ENTRY(threadCallWrapper)
        pushl 4(%esp)
        call 4(%esp)
        call exit

ENTRY(threadCreateWithStack)
        pushl %ebp
        movl %esp, %ebp
        pushl %ebx
        pushl %esi

        // paso de parametros
        movl 8(%ebp), %edx
        movl 12(%ebp), %ecx
        movl 16(%ebp), %ebx
        movl $threadCallWrapper, %esi

        // excepcion
        movl $25, %eax
	call syscall_sysenter
        popl %esi
	popl %ebx
	test %eax, %eax
	js nok
	popl %ebp
        ret

ENTRY(semCreate)
	pushl %ebp
	movl %esp, %ebp
	movl $26, %eax
	movl 8(%ebp), %edx;	//initial_value
	call syscall_sysenter
	test %eax, %eax
	js nok	// if (eax < 0) -->
	popl %ebp
	ret
        
ENTRY(semWait)
	pushl %ebp
	movl %esp, %ebp
	movl $27, %eax
	movl 8(%ebp), %edx;	//s
	call syscall_sysenter
	test %eax, %eax
	js nok	// if (eax < 0) -->
	popl %ebp
	ret
        
ENTRY(semSignal)
	pushl %ebp
	movl %esp, %ebp
	movl $28, %eax
	movl 8(%ebp), %edx;	//s
	call syscall_sysenter
	test %eax, %eax
	js nok	// if (eax < 0) -->
	popl %ebp
	ret
        
ENTRY(semDestroy)
	pushl %ebp
	movl %esp, %ebp
	movl $29, %eax
	movl 8(%ebp), %edx;	//s
	call syscall_sysenter
	test %eax, %eax
	js nok	// if (eax < 0) -->
	popl %ebp
	ret

ENTRY(memRegGet)
	pushl %ebp
	movl %esp, %ebp
	movl $31, %eax
	movl 8(%ebp), %edx;	//s
	call syscall_sysenter
	test %eax, %eax
	js nok	// if (eax < 0) -->
	popl %ebp
	ret

ENTRY(memRegDel)
	pushl %ebp
	movl %esp, %ebp
	movl $32, %eax
	movl 8(%ebp), %edx;	//s
	call syscall_sysenter
	test %eax, %eax
	js nok	// if (eax < 0) -->
	popl %ebp
	ret
