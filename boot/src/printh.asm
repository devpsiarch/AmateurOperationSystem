[bits 16]
;there must be a way to do this in a loop too lazy to do it
printh:
pusha 

mov si,hex_pattern

mov bx,dx
shr bx,12
mov bx,[bx+hex_table]
mov [hex_pattern + 2],bl 


mov bx,dx
shr bx,8 
and bx,0x000f           ;f:1111 use and to get only last 
mov bx,[bx+hex_table]
mov [hex_pattern + 3],bl 


mov bx,dx
shr bx,4 
and bx,0x000f          ;f:1111 use and to get only last 
mov bx,[bx+hex_table]
mov [hex_pattern + 4],bl 


mov bx,dx
shr bx,0 
and bx,0x000f           ;f:1111 use and to get only last 
mov bx,[bx+hex_table]
mov [hex_pattern + 5],bl 


call printf
    
popa
ret

hex_pattern: db '0x****',0x0a,0x0d,0
hex_table: db '0123456789abcdef',0  
