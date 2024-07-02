[org 0x7c00]
kernel_loc equ 0x1000

;es = ds = fs = gs = cs
mov ax, cs
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
;ss:sp = stack pointer 
mov bp, 0x9000
mov sp, bp

mov [driver_number],dl

mov si,msg_rm
call printf

call load_kernel 

call switch_protected_mode
 
jmp $
%include "src/cs16.asm" 
%include "src/printf32.asm"
%include "gdt.asm"
%include "load_gdt.asm"
%include "src/printf.asm"
%include "src/loaddisk.asm"
%include "load_kernel.asm"

[bits 32]
begin_protected_mode:

mov ebx,msg_pm
call printf32

jmp kernel_loc

jmp $
msg_rm:db 'booting from 16bit protected mode...',0x0a,0x0d,0 
msg_pm:db 'booted to 32bit protected mode',0
msg_lk:db 'loading kernel ...',0x0a,0x0d,0
driver_number:db 0

times 510-($-$$) db 0
dw 0xaa55
