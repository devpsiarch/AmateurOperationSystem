#ifndef IDT_H
#define IDT_H

#include "stdio.h"
struct idt_entry {
    U16 base_lo;
    U16 sel;        /*kernel segment goes here*/
    U8 always0;     /*This is always set to 0*/
    U8 flages;      /*Set using the above table*/
    U16 base_hi;
}__attribute__((packed));

struct idt_ptr {
    U16 limit;      /*As in size*/ 
    U32 base;       /*The whole package low and high*/
}__attribute__((packed));

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries in this tutorial, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */
struct idt_entry idt[256];
struct idt_ptr idtp;

//This func will exist in a seperate assembly file in src kernel
void idt_load();
#endif

#ifndef IDT_IMPLI
#define IDT_IMPLI

#endif
