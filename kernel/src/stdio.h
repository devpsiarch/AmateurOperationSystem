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



/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* All the I/O ports are calculated relative to the data port. This is because
     * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
     * order, but they start at different values.
     */

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 2)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)



#define SERIAL_LINE_ENABLE_DLAB         0x80
/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15


#define NULL 0
#define TRUE 1

#define FRAMEBUFFER_ADDRESS       0xB8000
#define FRAMEBUFFER_32BIT_ADDRESS 0xA0000

//for the printing strings and such hold chars ...
#define BUFFER_CAP 4000 

U16* fb;
int fb_size = 0;
static int fb_nli = 1; //next line index
void serial_configure_baud_rate(U16 com,U16 divisor);
void serial_configure_line(U16 com);
int serial_is_transmit_fifo_empty(unsigned int com);

U16 fb_cell(unsigned char ch, U8 fore_color, U8 back_color);
void fb_clear(U8 for_color,U8 back_color);
void vga_init(U8 for_color,U8 back_color);
void putchar(char c);
void let_char(char c,int loc);
void print_newline();
void print_string(char *str);

void fb_copy_row(int rs,int rd);
void mem_copy(char *dst,char *src,int size_bytes);
void outb(U16 port,U8 data);
U8 inb(U8 port);
void fb_move_cursor(unsigned short pos);
void wait_for_io(U32 timer_count);
void sleep(int timesleep);
void print_center(char *string);

#endif

#ifndef FRAMEBUFFER_IMPLI
#define FRAMEBUFFER_IMPLI

//NOW the C part of the implimention


//each cell of the framebuffer is a 16bit "cell" in a sense
//here we are creating a "cell" a 16 bit value that has : ASCII (8bits) + fg(4bits) + bg(4bits)

void serial_configure_baud_rate(U16 com,U16 divisor)
{
	outb(SERIAL_LINE_COMMAND_PORT(com),
		 SERIAL_LINE_ENABLE_DLAB);
	outb(SERIAL_DATA_PORT(com),
		 (divisor >> 8) & 0x00FF);
	outb(SERIAL_DATA_PORT(com),
		 divisor & 0x00FF);
}
void serial_configure_line(U16 com)
{
	/* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
	 * Content: | d | b | prty  | s | dl  |
	 * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
	 */
	outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

int serial_is_transmit_fifo_empty(unsigned int com)
{
	/* 0x20 = 0010 0000 */
	return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

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

//clears the buffer 
void fb_clear(U8 for_color,U8 back_color){
	for(int i = 0 ; i < BUFFER_CAP ; i++){
		(fb)[i] = fb_cell(NULL,for_color,back_color);
	}
	fb_size = 0;
	fb_move_cursor(fb_size);
	fb_nli = 1;
}

//init the framebuffer size and pointer 
void fb_init(U8 for_color,U8 back_color){
	fb = (U16*) FRAMEBUFFER_ADDRESS;
	fb_clear(for_color,back_color);
	global_for_color = for_color;
	global_back_color = back_color;	
}

//copies a rows of a framebuffer to another
//row src and row dst
void fb_copy_row(int rs,int rd){
	for(int i = 0 ; i < 80 ; i++){
		fb[rs*80+i] = fb[rd*80+i];
	}
}

//this is a scroll down methode , it decrements the current size of the buffer that points to the framebuffer !!!
void scroll_down(){
	for(int i = 0 ; i < 25 ; i++){
		fb_copy_row(i,i+1);
	}
	fb_size -= 80;
}



//overwrites a char in the framebuffer's next available cell 
void putchar(char c){
	if(fb_size == 80*25) {
		scroll_down();
	}
	fb[fb_size] = fb_cell(c,global_for_color,global_back_color);
	fb_size++;	
	fb_move_cursor(fb_size);
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
	fb_move_cursor(fb_size);
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


//copies the char bytes from src to dst only (dont change it idiot)
void mem_copy(char *dst,char *src,int size_bytes){
	for(int i = 0 ; i < size_bytes ; i++){
		dst[i] = src[i];
	}
}

void fb_move_cursor(unsigned short pos)
{
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb(FB_DATA_PORT,    pos & 0x00FF);
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
