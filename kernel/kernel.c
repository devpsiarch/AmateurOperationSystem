#include "./src/io.h"
#include "./src/page.h"

extern void main(void){
    enable_paging();
    fb_init(GREEN,BLACK);	
    fb_clear(GREEN,BLACK);
    if(init_serial_ports() == 1){
        print_string("Serial port is faulty !!");
    }
    char temp = 'A';
	for(int i = 0 ; i < 26 ;i++){
		for(int i = 0 ; i < 80 ; i++){
		putchar(temp);
		}
		temp ++;
	}
    print_string("Kernel is working fine !!!");

    return;
}
