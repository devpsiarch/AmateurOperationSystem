gdt:
gdt_null:
dq 0
 
gdt_code:
dw 0xffff       ;segment limit
dw 0            ;segment base
db 0            ;segment base
db 10011010b    ;access byte : present , privlage lvl , desc type (data/code) , exe (yes/no) ,
                ;direct/conforming , read/write , accessed (cpu sets this)
db 11001111b    ;7bits : granurality(4kib), size (protected mode), segment limit
db 0
 
gdt_data:
dw 0xffff       ;segment limit
dw 0            ;segment base
db 0            ;segment base
db 10010010b    ;access byte : present , privlage lvl , desc type (data/code) , exe (yes/no) ,
                ;direct/conforming , read/write , accessed (cpu sets this)
db 11001111b    ;7bits : granurality(4kib), size (protected mode), segment limit
db 0
 
gdt_end:
 
gdt_desc:
dw gdt_end-gdt-1    ;limit
dd gdt          ;base
 
CODE equ gdt_code - gdt
DATA equ gdt_data - gdt
