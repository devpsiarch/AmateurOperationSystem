read_str:
pusha

mov bx,si

find_end:
cmp byte [bx],0
je push_loop
inc bx
jmp find_end


push_loop:
mov al,[bx]
push ax
dec bx

cmp al,[si]
je read_stack

jmp push_loop

read_stack:
pop ax
cmp al,0
je exo_stack
mov ah,0x0e
int 0x10
jmp read_stack


exo_stack:
popa
ret
