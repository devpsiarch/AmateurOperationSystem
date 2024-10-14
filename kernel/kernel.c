#include "src/stdio.h"

extern void main(void){
	fb_init(GREEN,BLACK);	
    fb_clear(GREEN,BLACK);
	char temp = 'A';
	for(int i = 0 ; i < 26 ;i++){
		for(int i = 0 ; i < 80 ; i++){
		putchar(temp);
		}
		temp ++;
	}
    return;
}
