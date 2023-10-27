#include <asm.h>

ENTRY(get_fault_eip)
        push %ebp
        mov %esp, %ebp
        mov 60(%ebp), %eax
        mov %ebp, %esp
        pop %ebp
        ret
