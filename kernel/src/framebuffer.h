#ifndef FRAMEBUFFER_H
#define FEAMEBUFFER_H

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;


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

U8 global_for_color = WHITE;
U8 global_back_color = BLACK;


#include "./keyboard.h"

#define NULL 0
#define TRUE 1

#define FRAMEBUFFER_ADDRESS       0xB8000
#define FRAMEBUFFER_32BIT_ADDRESS 0xA0000

//for the printing strings and such hold chars ...
#define BUFFER_CAP 2200 

U16* fb;
int fb_size = 0;
static int fb_nli = 1; //next line index

U16 fb_cell(unsigned char ch, U8 fore_color, U8 back_color);
void fb_clear(U8 for_color,U8 back_color);
void vga_init(U8 for_color,U8 back_color);
void putchar(char c);
void let_char(char c,int loc);
void print_newline();
void print_string(char *str);
U8 inb(U16 port);
void outb(U16 port, U8 data);
char get_input_keyboard();
void wait_for_io(U32 timer_count);
void sleep(int timesleep);
char get_ASCII_char(char keyCode);
void test_input();
void print_center(char *string);

#endif

#ifndef FRAMEBUFFER_H 
#define FRAMEBUFFER_H

//NOW the C part of the implimention


//each cell of the framebuffer is a 16bit "cell" in a sense
//here we are creating a "cell" a 16 bit value that has : ASCII (8bits) + fg(4bits) + bg(4bits)
U16 fb_cell(unsigned char ch, U8 fore_color, U8 back_color) 
{
  U16 ax = 0;
  U8 ah = 0, al = 0;

  ah = back_color;
  ah <<= 4;
  ah |= fore_color;
  ax = ah;
  ax <<= 8;
  al = ch;
  ax |= al;

  return ax;
}

//clears the framebuffer 
void fb_clear(U8 for_color,U8 back_color){
	for(int i = 0 ; i < BUFFER_CAP ; i++){
		(fb)[i] = fb_cell(NULL,for_color,back_color);
	}
	fb_size = 0;
	fb_nli = 1;
}

//init the framebuffer size and pointer 
void fb_init(U8 for_color,U8 back_color){
	fb = (U16*) FRAMEBUFFER_ADDRESS;
	fb_clear(for_color,back_color);
	global_for_color = for_color;
	global_back_color = back_color;	
}

//overwrites a char in the framebuffer's next available cell 
void putchar(char c){
	fb[fb_size] = fb_cell(c,global_for_color,global_back_color);
	fb_size++;	
}

//overwrites any cell in the framebuffer
void let_char(char c,int loc){
	fb[loc] = fb_cell(c,global_for_color,global_back_color);
}

//obviously new line 
void print_newline(){
	if(fb_nli >= 55){
		fb_nli = 0 ;
		fb_clear(global_for_color,global_back_color);
	}
	fb_size = 80*fb_nli;
	fb_nli++;
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


void print_int(int num){
	char integer_string[degit_count(num)+1];
	itc(num,integer_string);
	print_string(integer_string);
}

//gets in from port and return it 
U8 inb(U16 port)
{
  U8 ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
  return ret;
}
//gets in from port and send to 
void outb(U16 port, U8 data)
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
void wait_for_io(U32 timer_count)
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

void print_center(char *string){
	int len = strlen(string);
	int left = (80 - len)/2;	
	for(int i = 0 ; i < left ; i++){
		putchar(' ');
	}
	print_string(string);
	print_newline();
}

#endif // END of the C part
