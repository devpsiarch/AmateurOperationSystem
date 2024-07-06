#define VM 0xb8000
#define WHITE_ON_BLACK 0X07


extern void k_clear(){
	char *vm = (char*) VM;
	for(int i = 0; i < 80*25*2;i++){
		vm[i] = ' ';
		i++;	
	}
}

extern void k_printf(char *str){
	char *vm = (char*) VM;
	char *ptr = str;
	while(*ptr != '\0'){
		*vm = *ptr;
		ptr++;
		vm+=2;
	}

}



extern void main(void){
	char *video_memory = (char*) VM;
	k_clear();
	k_printf("hello this is C talking");
	return;
}
