#ifndef IDT_H
#define IDT_H

#include "./stdio.h"

//This is the interupt descriptor table entry
struct idt_entry {
    U16 base_lo;
    U16 base_hi;
    U16 selector;   /* Our kernel segment goes here! */
    U8 always0;     /* This will ALWAYS be set to 0! */
    U8 flags;       /* Set using the above table! */
}__attribute__((packed));
//pointer to the idt
struct idt_ptr
{
    U16 limit;
    U32 base;
} __attribute__((packed));
//populate the idt variable with relavent values
void idt_set_gate(U8 num, U64 base, U16 sel, U8 flags);
void idt_install(void);
extern void idt_load();
#endif // !IDT_H



#ifndef IDT_IMPLI
#define IDT_IMPLI

//256 entries of idt , we wont need all of them now
strcut idt_entry IDT[256];
strcut idt_ptr IDTP;

void idt_set_gate(U8 num, U64 base, U16 sel, U8 flags){
    IDT[num].base_lo = base & 0xffff;
    IDT[num].base_hi = (base & 0xffff0000) >> 16;
    IDT[num].selector = sel;        /*KERNEL CODE SEGEMENT OFFSET*/
    IDT[nums].always0 = 0;          /*Hmmmm ...*/
    IDT[nums].flags = flags;         /*Interupt gate*/
}
//
void idt_install(void){
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &IDT;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&IDT, 0, sizeof(struct idt_entry) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate */
    idt_set_gate(0x21,0x64,0x08,0x8e);
    idt_set_gate(0x21,0x60,0x08,0x8e);
    /* Points the processor's internal register to the new IDT */
    idt_load(); 
}
#endif // !IDT_IMPLI
