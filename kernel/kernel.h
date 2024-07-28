#ifndef KERNEL_H
#define KERNEL_H

//some definitions
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;


//colors for text
enum vga_color {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARK_GREY,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    YELLOW,
    WHITE,
};


#define NULL 0

#define VGA_ADDRESS 0xB8000
#define VGA_32bit_ADDRESS 0xA0000

//for the printing strings and such hold chars ...
#define BUFFER_SIZE 2200 

uint16* vga_buffer;
int vga_index = 0;

//this was copied idk what it does , it just works lol
//nah it just constructs a VGA text mode
uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color) 
{
  uint16 ax = 0;
  uint8 ah = 0, al = 0;

  ah = back_color;
  ah <<= 4;
  ah |= fore_color;
  ax = ah;
  ax <<= 8;
  al = ch;
  ax |= al;

  return ax;
}

//clears the buffer that holds the content of the vga address
void vga_clear(uint16 **buffer, uint8 for_color,uint8 back_color){
	for(int i = 0 ; i < BUFFER_SIZE ; i++){
		(*buffer)[i] = vga_entry(NULL,for_color,back_color);
	}
}

//init the parameters for the vga mode 
void vga_init(uint8 for_color,uint8 back_color){
	vga_buffer = (uint16*) VGA_ADDRESS;
	vga_clear(&vga_buffer,for_color,back_color);	
}

void putchar(char c){
	vga_buffer[vga_index] = vga_entry(c,WHITE,BLACK);
	vga_index++;	
}

//clears screen
void k_clear(){
        char *vm = (char*) VGA_ADDRESS;
        for(int i = 0; i < 80*25*2;i++){
                vm[i] = ' ';
                i++;
        }
}

//prints a string from vga address
void printf_string(unsigned char *str){
        char *vm = (char*) VGA_ADDRESS;
        char *ptr = str;
        while(*ptr != '\0'){
                *vm = *ptr;
                ptr++;
                vm+=2;
        }

}




#endif 
