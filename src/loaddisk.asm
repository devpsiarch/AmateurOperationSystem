[bits 16]
;on return al status on https://stanislavs.org/helppc/int_13-1.html
load_disk:
push dx
;dh contains number of secs
;and al contains boot drive number
;we push to save the value for later

mov ah,0x02   ;BIOS mode
mov al,dh   ;number of sectors to read

mov ch,0x0    ;cylinder number
mov cl,0x02    ;sector number


mov dh,0x0	;head number
mov dl,[driver_number]



int 0x13
;after the read:
;ah holds the status
;al holds number of sectors read
;more on 
;https://stanislavs.org/helppc/int_13-2.htmljc 

;checking for errors 
jc ferr

pop dx
cmp al,dh
jne sectors_read_error
cmp ah,00
jne status_error
jmp load_disk_done


ferr:
mov si,flage_set_msg
call printf
jmp load_disk_done

sectors_read_error:
mov si,read_failed_msg
call printf
jmp load_disk_done

status_error:
mov si,status_error_msg
call printf
jmp load_disk_done

load_disk_done:
ret

status_error_msg:db 'return status of ah register not 0 , error has accured while reading!',0x0a,0x0d,0
read_failed_msg:db 'failed to read all sectors defined!',0x0a,0x0d,0
flage_set_msg:db 'flages set error while reading disk!',0x0a,0x0d,0
