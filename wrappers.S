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
	popl %ebp
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
        pushl %ebp
        movl %esp, %ebp

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
	pushl %ebx;  // Save EBX, ESI and EDI if modified
	movl $35, %eax
	movl 8(%ebp), %ebx;	//pid
	movl 12(%ebp), %ecx;	//st
	call syscall_sysenter
	popl %ebx
	test %eax, %eax
	js nok	// if (eax < 0) -->
	popl %ebp
	ret

<<<<<<< HEAD
ENTRY(waitKey)
	pushl %ebp
	movl %esp, %ebp
	pushl %ebx;  // Save EBX, ESI and EDI if modified
	movl $11, %eax
	movl 8(%ebp), %ebx;	//b*
	movl 12(%ebp), %ecx;	//timeout
	call syscall_sysenter
	popl %ebx
	test %eax, %eax
	js nok	// if (eax < 0) -->
	popl %ebp
	ret
=======
        // paso de parametros
        movl 8(%ebp), %edx
        movl 12(%ebp), %ecx

        // excepcion
        movl $35, %eax
	push %ecx
	push %edx
	push $cont_get_stats
	push %ebp
	mov %esp, %ebp
	sysenter
cont_get_stats:
	pop %ebp
	addl $4, %esp
	pop %edx
	pop %ecx

        cmpl $0, %eax
        jge end_get_stats
err_get_stats:
        neg %eax
        movl %eax, errno
        movl $-1, %eax
end_get_stats:
        movl %ebp, %esp
        popl %ebp
        ret


ENTRY(waitKey)
        pushl %ebp
        movl %esp, %ebp

        // paso de parametros
        movl 8(%ebp), %edx
        movl 12(%ebp), %ecx

        // excepcion
        movl $11, %eax
	push %ecx
	push %edx
	push $cont_waitKey
	push %ebp
	mov %esp, %ebp
	sysenter
cont_waitKey:
	pop %ebp
	addl $4, %esp
	pop %edx
	pop %ecx

        cmpl $0, %eax
        jge end_waitKey
err_waitKey:
        neg %eax
        movl %eax, errno
        movl $-1, %eax
end_waitKey:    
        movl %ebp, %esp
        popl %ebp
        ret
>>>>>>> final
