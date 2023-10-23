#include <asm.h>

ENTRY(task_switch)
        push %ebp
        mov %esp, %ebp

        push %esi
        push %edi
        push %ebx
        
        push 8(%ebp)
        call inner_task_switch
        add $4, %esp
        
        pop %ebx
        pop %edi
        pop %esi

        mov %ebp, %esp
        pop %ebp
        ret
        
ENTRY(get_ebp)
        push %ebp
        mov %esp, %ebp
        
        mov %ebp, %eax
        
        mov %ebp, %esp
        pop %ebp
        ret

ENTRY(set_esp)
        mov 8(%esp), %esp
        pop %ebp
        ret
