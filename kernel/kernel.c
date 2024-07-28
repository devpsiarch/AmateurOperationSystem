#include "kernel.h"


extern void main(void){
	vga_init(WHITE,BLUE);

	putchar('c');
	putchar('x');
	putchar('c');
	putchar('x');
	clear_screen();	
	print_string("hello friends");
	print_newline();
	print_string("i am from the futur and your rich");
	print_newline();	
	print_integer(12345);
	print_newline();	

	return;
}
