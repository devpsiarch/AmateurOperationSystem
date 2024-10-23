# Loads the IDT defined in '_idtp' into the processor.
# This is declared in C as 'extern void idt_load();'
.global load_idt
.extern idtp
load_idt:
    lidt [idtp]
    ret
