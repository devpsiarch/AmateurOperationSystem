#; Loads the IDT defined in '_idtp' into the processor.
#; This is declared in C as 'extern void idt_load();'
.intel_syntax noprefix
.global _idt_load
_idt_load:
    mov edx , [esp + 4]
    lidt [edx]
    sti
    ret
