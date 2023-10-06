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
        pushl %ebx

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
	addl $4, %ebp
	pop %edx
	pop %ecx

        cmpl $0, %eax
        jge end_gettime
err_gettime:
        neg %eax
        movl %eax, errno
        movl $-1, %eax
end_gettime:    
        popl %ebx
        movl %ebp, %esp
        popl %ebp
        ret
