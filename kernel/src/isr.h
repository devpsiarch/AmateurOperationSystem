#ifndef ISR_H
#define ISR_H
// Am so sorry for this scene ... i think its the only way so just deal with it ...
// these are all the 32 entries in out IDT that will hopfully handle the interrupts 
extern void isr_0();
extern void isr_1();
extern void isr_2();
extern void isr_3();
extern void isr_4();
extern void isr_5();
extern void isr_6();
extern void isr_7();
extern void isr_8();
extern void isr_9();
extern void isr_10();
extern void isr_11();
extern void isr_12();
extern void isr_13();
extern void isr_14();
extern void isr_15();
extern void isr_16();
extern void isr_17();
extern void isr_18();
extern void isr_19();
extern void isr_20();
extern void isr_21();
extern void isr_22();
extern void isr_23();
extern void isr_24();
extern void isr_25();
extern void isr_26();
extern void isr_27();
extern void isr_28();
extern void isr_29();
extern void isr_30();
extern void isr_31();
//This will init all the 32 entries above to the IDT , so enjoy more copy past !!!
void isrs_install();
//This DS holds the previous state of the CPU , i dont remember why we would need this but hey 
struct regs
{
    U32 gs, fs, es, ds;      /* pushed the segs last */
    U32 edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    U32 int_no, err_code;    /* our 'push byte #' and ecodes do this */
    U32 eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

//main fault handler that will be called from assembly
extern void fault_handler(struct regs *r);
#endif // interrupt service routines bassicly they handle exeptions and interrupts
#ifndef ISR_IMPLI
#define ISR_IMPLI

void isrs_install(){
    idt_set_gate(0, (unsigned)isr_0, 0x08, 0x8E);
    idt_set_gate(1, (unsigned)isr_1, 0x08, 0x8E);
    idt_set_gate(2, (unsigned)isr_2, 0x08, 0x8E);
    idt_set_gate(3, (unsigned)isr_3, 0x08, 0x8E);
    idt_set_gate(4, (unsigned)isr_4, 0x08, 0x8E);
    idt_set_gate(5, (unsigned)isr_5, 0x08, 0x8E);
    idt_set_gate(6, (unsigned)isr_6, 0x08, 0x8E);
    idt_set_gate(7, (unsigned)isr_7, 0x08, 0x8E);
    idt_set_gate(8, (unsigned)isr_8, 0x08, 0x8E);
    idt_set_gate(9, (unsigned)isr_9, 0x08, 0x8E);
    idt_set_gate(10, (unsigned)isr_10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned)isr_11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned)isr_12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned)isr_13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned)isr_14, 0x08, 0x8E);
    idt_set_gate(15, (unsigned)isr_15, 0x08, 0x8E);
    idt_set_gate(16, (unsigned)isr_16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned)isr_17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned)isr_18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned)isr_19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned)isr_20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned)isr_21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned)isr_22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned)isr_23, 0x08, 0x8E);
    idt_set_gate(24, (unsigned)isr_24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned)isr_25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned)isr_26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned)isr_27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned)isr_28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned)isr_29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned)isr_30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned)isr_31, 0x08, 0x8E);
}
//This array will prolly be used for testing if the interrupt handler is working fine or not
const char *exceptions[] = {
    "Division By Zero Exception",
    "Debug Exception",
    "Non Maskable Interrupt Exception",
    "Breakpoint Exception",
    "Into Detected Overflow Exception",
    "Out of Bounds Exception",
    "Invalid Opcode Exception",
    "No Coprocessor Exception",
    "Double Fault Exception",
    "Coprocessor Segment Overrun Exception",
    "Bad TSS Exception",
    "Segment Not Present Exception",
    "Stack Fault Exception",
    "General Protection Fault Exception",
    "Page Fault Exception",
    "Unknown Interrupt Exception",
    "Coprocessor Fault Exception",
    "Alignment Check Exception (486+)",
    "Machine Check Exception (Pentium/586+)",
    "Reserved Exceptions",
    "Reserved Exceptions",
    "Reserved Exceptions",
    "Reserved Exceptions",
    "Reserved Exceptions",
    "Reserved Exceptions",
    "Reserved Exceptions",
    "Reserved Exceptions",
    "Reserved Exceptions",
    "Reserved Exceptions",
    "Reserved Exceptions",
    "Reserved Exceptions",
    "Reserved Exceptions"
};

extern void fault_handler(struct regs *r){
    /* Is this a fault whose number is from 0 to 31? */
    if (r->int_no < 32)
    {
        /* Display the description for the Exception that occurred.
        *  In this tutorial, we will simply halt the system using an
        *  infinite loop */
        puts(exception_messages[r->int_no]);
        puts(" Exception. System Halted!\n");
        for (;;);
    }
}
#endif // !ISR_IMPLI
