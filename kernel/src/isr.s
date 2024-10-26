;these macros will hadnle the interrupts much cleaner ... 
;instead if defining all the ISR's we can use these macros that declares one on its own   
global fault_handler
%macro no_error_handler 1
    global isr_%1 
    isr_%1:
        push dword 0        ;pushes 0 as the "error" status code
        push dword %1       ;pushes the interrupt code
        jmp main_interrupt_handler
%endmacro
%macro error_handler 1
    global isr_%1 
    isr_%1:
        push dword %1       ;pushes the interrupt code only cuz the error status is already pushed 
        jmp main_interrupt_handler
%endmacro
;this function is the main ISR , it has to save all the registers and call the C function
main_interrupt_handler:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, fault_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!
; Division By Zero Exception
no_error_handler 0
; Debug Exception
no_error_handler 1
; Non Maskable Interrupt Exception
no_error_handler 2
; Breakpoint Exception
no_error_handler 3
; Into Detected Overflow Exception
no_error_handler 4
; Out of Bounds Exception
no_error_handler 5
; Invalid Opcode Exception
no_error_handler 6
; No Coprocessor Exception
no_error_handler 7
; Double Fault Exception
error_handler 8
; Coprocessor Segment Overrun Exception
no_error_handler 9
; Bad TSS Exception
error_handler 10
; Segment Not Present Exception
error_handler 11
; Stack Fault Exception
error_handler 12
; General Protection Fault Exception
error_handler 13
; Page Fault Exception
error_handler 14
; Unknown Interrupt Exception
no_error_handler 15
; Coprocessor Fault Exception
no_error_handler 16
; Alignment Check Exception (486+)
no_error_handler 17
; Machine Check Exception (Pentium/586+)
no_error_handler 18
; Reserved Exceptions
no_error_handler 19
no_error_handler 20
no_error_handler 21
no_error_handler 22
no_error_handler 23
no_error_handler 24
no_error_handler 25
no_error_handler 26
no_error_handler 27
no_error_handler 28
no_error_handler 29
no_error_handler 30
no_error_handler 31
