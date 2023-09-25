#include <asm.h>

ENTRY(addAsm)
        push %ebp
        mov %esp, %ebp
        mov 0x8(%ebp), %eax
        add 0xC(%ebp), %eax
        mov %ebp, %esp
        pop %ebp
        ret
