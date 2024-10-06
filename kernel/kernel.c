#include "utils/kernel.h"

extern void main(void){
	vga_init(GREEN,BLACK);	
	clear_screen();
	print_string("hello father...");
	return;
}
