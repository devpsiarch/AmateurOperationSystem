#ifndef PAGE_H 
#define PAGE_H
#include "io.h"
//TODO ==> This if a temporaty solution to help me out , I should write a proper page allocator later on. 
U32 page_directory[1024] __attribute__((aligned(4096)));
U32 page_table[1024] __attribute__((aligned(4096)));

void init_page_directory(void);
void init_page_table(void);

extern void loadPageDirectory(U32*);
extern void enablePaging();

void enable_paging(void);

#endif /* ifndef  */
#ifndef PAGE_IMPLI
#define PAGE_IMPLI
//sets every entry to not present (ie not intented to be used)
void init_page_directory(void){
    for(int i = 0 ; i < 1024;i++){
    // This sets the following flags to the pages:
    //   Supervisor: Only kernel-mode can access them
    //   Write Enabled: It can be both read from and
        page_directory[i] = 0x00000002;
    }
}

//Will map all 1024 entries , mapping 4 megabytes         
void init_page_table(void){
    for(int i = 0 ; i < 1024 ; i++){
            // As the address is page aligned, it will always leave 12 bits zeroed.
            // Those bits are used by the attributes = Supervisor level , read/read present
        page_table[i] = (i * 0x1000) | 3;
    }
    page_directory[0] = ((U32)page_table) | 3;
}

void enable_paging(void){
    init_page_directory();
    init_page_table();
    loadPageDirectory(page_directory);
    enablePaging();  
}
#endif // !PAGE_IMPLI
