[bits 16]
load_kernel:
mov si,msg_lk
call printf

mov bx,kernel_loc
mov dh,50

call load_disk 

ret
