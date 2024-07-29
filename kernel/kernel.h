#ifndef KERNEL_H
#define KERNEL_H

#include "keyboard.h"

//some definitions
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

//ascii code for degits 0 -> 9
int ASCII_digit[10] = {
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39
};

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
#define TRUE 1

#define VGA_ADDRESS 0xB8000
#define VGA_32bit_ADDRESS 0xA0000

//for the printing strings and such hold chars ...
#define BUFFER_SIZE 2200 

uint16* vga_buffer;
int vga_index = 0;
static int next_line_index = 1;


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
	vga_index = 0;
	next_line_index = 1;
}

//init the parameters for the vga mode 
void vga_init(uint8 for_color,uint8 back_color){
	vga_buffer = (uint16*) VGA_ADDRESS;
	vga_clear(&vga_buffer,for_color,back_color);	
}

//prints char
void putchar(char c){
	vga_buffer[vga_index] = vga_entry(c,WHITE,BLACK);
	vga_index++;	
}

//clears screen
void clear_screen(){
        char *vm = (char*) VGA_ADDRESS;
        for(int i = 0; i < 80*25*2;i++){
                vm[i] = ' ';
                i++;
        }
		vga_clear(&vga_buffer,WHITE,BLACK);
}

//obviously new line 
void print_newline(){
	if(next_line_index >= 55){
		next_line_index = 0 ;
		vga_clear(&vga_buffer,WHITE,BLACK);
	}
	vga_index = 80*next_line_index;
	next_line_index++;
}

//prints string duh
void print_string(char *str){
	while(*str != '\0'){
		putchar(*str);
		str++;
	}
}


int strlen(char *str){
	int size = 0;
	while(*str != '\0'){
		size++;
		str++;
	}	
	return size;
}


int degit_count(int num){
	if(num == 0){
		return 1;
	}
	int count = 0;
	while(num > 0){
		count ++;
		num /= 10; 
	}
	return count;
}

void itc(int num,char *str_number){
	int dgcount = degit_count(num);
	int index = dgcount - 1;
	char x;
	if(num == 0 && dgcount == 1){
		str_number[0] = '0';
		str_number[1] = '\0';
	}else{
		while(num != 0){
			x = num % 10;
			str_number[index] = x + '0';
			index--;
			num /= 10;
		}
		str_number[dgcount] = '\0';
	}
}


void print_integer(int num){
	char integer_string[degit_count(num)+1];
	itc(num,integer_string);
	print_string(integer_string);
}

//gets in from port and return it 
uint8 inb(uint16 port)
{
  uint8 ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
  return ret;
}
//gets in from port and send to 
void outb(uint16 port, uint8 data)
{
  asm volatile("outb %0, %1" : "=a"(data) : "d"(port));
}


char get_input_keyboard(){
	char ch = 0;
	while((ch = inb(KEYBOARD_PORT)) != 0){
		if(ch > 0){
			return ch;
		}	
	}
	return ch;
}


//waits , gets the CPU busy to od nothing whatsoever
void wait_for_io(uint32 timer_count)
{
  while(1){
    asm volatile("nop");
    timer_count--;
    if(timer_count <= 0)
      break;
    }
}
//sleeps lol
void sleep(int timesleep){
	wait_for_io(timesleep);
}

char get_ASCII_char(char keyCode) {
    switch (keyCode) {
        case KEY_A:
            return 'A';
        case KEY_B:
            return 'B';
        case KEY_C:
            return 'C';
        case KEY_D:
            return 'D';
        case KEY_E:
            return 'E';
        case KEY_F:
            return 'F';
        case KEY_G:
            return 'G';
        case KEY_H:
            return 'H';
        case KEY_I:
            return 'I';
        case KEY_J:
            return 'J';
        case KEY_K:
            return 'K';
        case KEY_L:
            return 'L';
        case KEY_M:
            return 'M';
        case KEY_N:
            return 'N';
        case KEY_O:
            return 'O';
        case KEY_P:
            return 'P';
        case KEY_Q:
            return 'Q';
        case KEY_R:
            return 'R';
        case KEY_S:
            return 'S';
        case KEY_T:
            return 'T';
        case KEY_U:
            return 'U';
        case KEY_V:
            return 'V';
        case KEY_W:
            return 'W';
        case KEY_X:
            return 'X';
        case KEY_Y:
            return 'Y';
        case KEY_Z:
            return 'Z';
        case KEY_1:
            return '1';
        case KEY_2:
            return '2';
        case KEY_3:
            return '3';
        case KEY_4:
            return '4';
        case KEY_5:
            return '5';
        case KEY_6:
            return '6';
        case KEY_7:
            return '7';
        case KEY_8:
            return '8';
        case KEY_9:
            return '9';
        case KEY_0:
            return '0';
		case KEY_SPACE:
		   return ' ';	
		default:
            return '\0'; // Return null character for unknown keycodes
    }
}

void test_input(){
	char ch = 0;
	char keycode = 0;

	do{
		keycode = get_input_keyboard();
		if(keycode == KEY_ENTER){
			print_newline();
		}else{
			ch = get_ASCII_char(keycode);
			if('\0'){
				putchar('!');
				continue;
			}
			putchar(ch);
		}
		sleep(0x02FFFFFF);
	}while(ch > 0);
}


#endif 
