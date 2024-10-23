#ifndef IDT_H
#define IDT_H
//Headers part of the code 
#include "io.h"
// The idt if a table that tells the OS what ISR to use for each interrupt
#define IDT_SIZE 256
struct idt_entry {
   U16 base_lo;        // offset bits 0..15
   U16 selector;        // a code segment selector in GDT or LDT
   U8  zero;            // unused, set to 0
   U8  flags; // gate type, dpl, and p fields
   U16 base_hi;        // offset bits 16..31
}__attribute__((packed));

struct idt_ptr {
    U16 limit;
    U32 base;
}__attribute__((packed));

extern void load_idt();

//sets a entry in the idt 
void idt_set_gate(U8 num, U64 base, U16 sel, U8 flags);
void idt_install(void);
#endif





#ifndef IDT_IMPLI
#define IDT_IMPLI
//C part of the code
struct idt_entry idt[IDT_SIZE];
struct idt_ptr idtp;

//sets the idt for the ISR 
void idt_set_gate(U8 num, U64 base, U16 sel, U8 flags){
    // Interrupt routine base pointer
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    //Other stuff : access flags , segment used
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

/* Installs the IDT */
void idt_install(void)
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit = (sizeof (struct idt_entry) * IDT_SIZE) - 1;
    idtp.base = (U32)&idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idt, 0, sizeof(struct idt_entry) * IDT_SIZE);

    /* Add any new ISRs to the IDT here using idt_set_gate */

    /* Points the processor's internal register to the new IDT */
    load_idt();
}
	

#endif
