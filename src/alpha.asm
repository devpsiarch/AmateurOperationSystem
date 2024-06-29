print_alpha:
pusha

mov ah,0x0e
mov al,97


rec_alpha:
int 0x10
inc al
test al,1
jz odd
jmp even 


even:
add al,32
cmp al,123
je done
jmp rec_alpha

odd:
sub al,32
cmp al,91
je done
jmp rec_alpha

done:
popa
ret
