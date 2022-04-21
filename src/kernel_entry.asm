section .text
[bits 32]
    
[extern _start]
call _start		        ; calls kernel function start()

jmp $

%include "src/cpu/interrupt.asm"

section .rodata		    ; read only data
