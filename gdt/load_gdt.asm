[bits 16]
switch_protected_mode:
cli
lgdt [gdt_desc]
 
mov eax,cr0
or eax,0x1
mov cr0,eax
 
jmp CODE:init_pm
 
[bits 32]
init_pm:
mov ax,DATA
mov ds,ax
mov ss,ax
mov es,ax
mov fs,ax
mov gs,ax
 
mov ebp,0x90000
mov esp,ebp
 
call begin_protected_mode
