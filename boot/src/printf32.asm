[bits 32]
;string stored in ebx
printf32:
pusha
mov edx,0xb8000

print_loop:

mov al,[ebx]
cmp al,0 
je end_print

mov ah,0x0f
mov [edx],ax
add edx,2
add ebx,1
jmp print_loop


end_print:
popa
ret
