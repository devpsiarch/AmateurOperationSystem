inputchar:
pusha 

mov bx,buffer
mov cx,10


READ:
cmp cx,1
je OVERFLOW

mov ah,0
int 0x16

cmp al,13
je END_READ

mov [bx],al
mov ah,0x0e
int 0x10
inc bx
sub cx,1
jmp READ


OVERFLOW:
mov si,eSIZE
call printf
jmp END_READ


END_READ:
xor ax,ax
mov [bx],ax
popa
ret

mov si,buffer
call printf

buffer:times 10 db 0
eSIZE:db 'size exceeded size of string!',0x0a,0x0d,0 
