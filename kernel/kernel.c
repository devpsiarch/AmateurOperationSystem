#include "kernel.h"


extern void main(void){
	vga_init(WHITE,BLUE);
	print_center("Welcome to my OS");

	return;
}
