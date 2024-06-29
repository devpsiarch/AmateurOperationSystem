;on return al status on https://stanislavs.org/helppc/int_13-1.html
load_disk:
mov ah,02   ;BIOS mode
mov al,1    ;number of sectors to read

mov dx,ax
push dx     ;to save tyhe values for after return

mov ch,0    ;cylinder number
mov cl,2    ;sector number

mov dh,0    ;head number
mov dl,[drive_number] ;selfexpli

mov bx,0x8112

int 0x13
;after the read:
;ah holds the status
;al holds number of sectors read
;more on 
;https://stanislavs.org/helppc/int_13-2.htmljc ferr

;checking for errors 
jc ferr

pop dx
cmp al,dl
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
