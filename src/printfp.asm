[bits 32]
video_memory equ 0xb8000;address of current VGA mode
bow equ 0x0f            ;black on white attributes

;NULL TERMINATED str on edx
print_str:
pusha

mov edx,video_memory

strloop:
mov al,[ebx]
mov ah,bow

cmp al,0
je done

add ebx,1
add edx,2

jmp strloop

done:
popa
ret
