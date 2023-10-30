#include <asm.h>

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
	push %ecx
	push %edx
	push $cont_write
	push %ebp
	mov %esp, %ebp
	sysenter
cont_write:
	pop %ebp
	addl $4, %esp
	pop %edx
	pop %ecx

        cmpl $0, %eax
        jge end_write
err_write:
        neg %eax
        movl %eax, errno
        movl $-1, %eax
end_write:    
        popl %ebx
        movl %ebp, %esp
        popl %ebp
        ret

ENTRY(gettime)
        pushl %ebp
        movl %esp, %ebp

        // excepcion
        movl $10, %eax
	push %ecx
	push %edx
	push $cont_gettime
	push %ebp
	mov %esp, %ebp
	sysenter
cont_gettime:
	pop %ebp
	addl $4, %esp
	pop %edx
	pop %ecx

        cmpl $0, %eax
        jge end_gettime
err_gettime:
        neg %eax
        movl %eax, errno
        movl $-1, %eax
end_gettime:
        movl %ebp, %esp
        popl %ebp
        ret

ENTRY(getpid)
        pushl %ebp
        movl %esp, %ebp

        // excepcion
        movl $20, %eax
	push %ecx
	push %edx
	push $cont_getpid
	push %ebp
	mov %esp, %ebp
	sysenter
cont_getpid:
	pop %ebp
	addl $4, %esp
	pop %edx
	pop %ecx

        cmpl $0, %eax
        jge end_getpid
err_getpid:
        neg %eax
        movl %eax, errno
        movl $-1, %eax
end_getpid:    
        movl %ebp, %esp
        popl %ebp
        ret
        
ENTRY(fork)
        pushl %ebp
        movl %esp, %ebp

        // excepcion
        movl $2, %eax
	push %ecx
	push %edx
	push $cont_fork
	push %ebp
	mov %esp, %ebp
	sysenter
cont_fork:
	pop %ebp
	addl $4, %esp
	pop %edx
	pop %ecx

        cmpl $0, %eax
        jge end_fork
err_fork:
        neg %eax
        movl %eax, errno
        movl $-1, %eax
end_fork:    
        movl %ebp, %esp
        popl %ebp
        ret

ENTRY(exit)
        pushl %ebp
        movl %esp, %ebp

        // excepcion
        movl $1, %eax
	push %ecx
	push %edx
	push $cont_exit
	push %ebp
	mov %esp, %ebp
	sysenter
cont_exit:
	pop %ebp
	addl $4, %esp
	pop %edx
	pop %ecx
end_exit:    
        movl %ebp, %esp
        popl %ebp
        ret

ENTRY(get_stats)
        pushl %ebp
        movl %esp, %ebp

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
