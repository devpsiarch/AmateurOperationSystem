[org 0x7c00]
kernel_loc equ 0x1000

;es = ds = fs = gs = cs
mov ax, cs
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
;ss:sp = stack pointer 
;Stack for the bootloader
mov bp, 0x9000
mov sp, bp

mov [driver_number],dl

mov si,msg_rm
call printf

call load_kernel 

call switch_protected_mode
 
jmp $
%include "boot/src/cs16.asm" 
%include "boot/src/printf32.asm"
%include "boot/gdt/gdt.asm"
%include "boot/gdt/load_gdt.asm"
%include "boot/src/printf.asm"
%include "boot/src/loaddisk.asm"
%include "kernel/load_kernel.asm"

[bits 32]
begin_protected_mode:

mov ebx,msg_pm
call printf32

;;setting up the kernel stack
KERNEL_STACK_SIZE equ 4069
section .bss
align 4
KERNEL_STACK: resb KERNEL_STACK_SIZE
section .text

jmp kernel_loc

jmp $
msg_rm:db 'booting from 16bit protected mode...',0x0a,0x0d,0 
msg_pm:db 'booted to 32bit protected mode',0
msg_lk:db 'loading kernel ...',0x0a,0x0d,0
driver_number:db 0

times 510-($-$$) db 0
dw 0xaa55
