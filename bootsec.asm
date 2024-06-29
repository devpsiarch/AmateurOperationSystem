[bits 16]
[org 0x7c00]

gdt:
gdt_null:
dq 0

gdt_code:
dw 0FFFFh       ;segment limit
dw 0            ;segment base
db 0            ;segment base
db 10011010b    ;access byte : present , privlage lvl , desc type (data/code) , exe (yes/no) , 
                ;direct/conforming , read/write , accessed (cpu sets this) 
db 11001111b    ;7bits : granurality(4kib), size (protected mode), segment limit
db 0
gdt_data:
dw 0FFFFh       ;segment limit
dw 0            ;segment base
db 0            ;segment base
db 10010010b    ;access byte : present , privlage lvl , desc type (data/code) , exe (yes/no) , 
                ;direct/conforming , read/write , accessed (cpu sets this) 
db 11001111b    ;7bits : granurality(4kib), size (protected mode), segment limit
db 0

gdt_end:

gdt_desc:
dw ($-gdt-1)    ;limit
dd gdt              ;base


cli
xor ax,ax
mov ds,ax

lgdt [gdt]

mov eax, cr0
or eax, 1
mov cr0, eax

jmp 08h:clear_pipe

[bits 32]
clear_pipe:

mov ax, 08h
mov ds, ax
mov ss, ax

mov ebp,0x90000
mov esp,ebp


mov eax, 0B8000h      ; Load the base address of VGA text buffer into eax
mov byte [eax], 'P'   ; Store 'P' at the beginning of the text buffer
mov byte [eax+1], 1Bh ;

hang:
    jmp hang

times 510-($-$$) db 0
dw 0xAA55
