[bits 16]
printf:
    pusha
    mov ah,0x0e
    putchar:
        mov al,[si]
        cmp al,0
        jne loop_str
        popa
        ret
    loop_str:
        int 0x10
        add si,1
        jmp putchar     

