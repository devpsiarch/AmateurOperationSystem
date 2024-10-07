#include "src/kernel.h"

extern void main(void){
	fb_init(GREEN,BLACK);	
	fb_clear(GREEN,BLACK);
    let_char('A',80);
    print_string("hello my friends");
    return;
}
