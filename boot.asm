[bits 16]
[org 0x7c00]


; es = ds = fs = gs = cs
mov ax, cs
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
; ss:sp = stack pointer 
mov sp, 0x50
mov ss, sp
mov sp, 0x7700

call switch_protected_mode
 
jmp $
 
;%include "src/printf.asm"
;%include "src/printfp.asm"
%include "gdt.asm"
%include "protected_mode.asm"
 
[bits 32]
begin_protected_mode:
 
 
mov al,'A'
mov ah,0x0f
mov [0xb8000],ax
 
jmp $
 
times 510-($-$$) db 0
dw 0xaa55
