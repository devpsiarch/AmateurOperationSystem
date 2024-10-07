#ifndef IO_H 
#define IO_H

#define FRAME_BUFFER 0xB8000
// fg refers to the foreground color of the cell while bg to the background
void fb_write_cell(unsigned int loc,char ascii,unsigned char fg,unsigned char bg);

#endif // The header part




#ifndef IO_IMPLI
#define IO_IMPLI


#endif // The C part 
