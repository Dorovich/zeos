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
        int $0x80

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
        int $0x80

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
