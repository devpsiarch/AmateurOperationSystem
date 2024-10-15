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

#define IDT_SIZE 256

//256 entries of idt , we wont need all of them now
struct idt_entry IDT[IDT_SIZE];

struct idt_ptr IDTP;

void idt_set_gate(U8 num, U64 base, U16 sel, U8 flags){
    IDT[num].base_lo = base & 0xffff;
    IDT[num].base_hi = (base & 0xffff0000) >> 16;
    IDT[num].selector = sel;        /*KERNEL CODE SEGEMENT OFFSET*/
    IDT[num].always0 = 0;          /*Hmmmm ...*/
    IDT[num].flags = flags;         /*Interupt gate*/
}
//
void idt_install(void){
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    IDTP.limit = (sizeof (struct idt_entry) * IDT_SIZE) - 1;
    //IDTP.base = &IDT;

    ////////////////////
    /* Down bellow looks important so ...*/

    /*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	write_port(0x20 , 0x11);
	write_port(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	write_port(0x21 , 0x20);
	write_port(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	write_port(0x21 , 0x00);  
	write_port(0xA1 , 0x00);  

	/* ICW4 - environment info */
	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	write_port(0x21 , 0xff);
	write_port(0xA1 , 0xff);


    /* Clear out the entire IDT, initializing it to zeros */
    memset(&IDT, 0, sizeof(struct idt_entry) * IDT_SIZE);

    /* Add any new ISRs to the IDT here using idt_set_gate */
    idt_set_gate(0x21,0x64,0x08,0x8e);
    idt_set_gate(0x21,0x60,0x08,0x8e);

    /* Points the processor's internal register to the new IDT */
    idt_load(IDTP); 
}
#endif // !IDT_IMPLI
