#include "src/kernel.h"

extern void main(void){
	fb_init(GREEN,BLACK);	
    fb_clear(GREEN,BLACK);

    print_string("hello my friends");
	print_newline();
    return;
}
