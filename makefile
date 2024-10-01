all:OS	


file=kernel
KERNEL_FILES := $(wildcard kernel/*.asm)
KERNEL_UTILS := $(wildcard kernel/utils/*.h)

BOOT_FILE := boot.asm
ZERO_FILE := zeros.asm

#BOOT_BIN needs all in gdt , src , kernel
BOOT_BIN := $(wildcard gdt/*.asm src/*.asm)


bin/boot.bin:$(BOOT_BIN) $(KERNEL_FILES)
	nasm -f bin boot.asm -o bin/boot.bin	

bin/zeros.bin:$(ZERO_FILE)
	nasm -f bin zeros.asm -o bin/zeros.bin

obj/kernel_entry.o:$(KERNEL_FILES)
	nasm kernel/kernel_entry.asm -f elf32 -o obj/kernel_entry.o

#probebly where us should add if u added files to kernel
obj/kernel.o:kernel/kernel.c $(KERNEL_UTILS) 
	 gcc -fno-pie -ffreestanding -c -m32 kernel/kernel.c -o obj/kernel.o

# i dont think ill need to refactor much code here 

bin/full_kernel.bin:obj/kernel.o obj/kernel_entry.o
	ld -o bin/full_kernel.bin -e main -m elf_i386 -s -Ttext 0x1000 obj/kernel_entry.o obj/kernel.o --oformat binary

bin/OS.bin:bin/boot.bin bin/full_kernel.bin bin/zeros.bin
	cat bin/boot.bin bin/full_kernel.bin bin/zeros.bin > bin/OS.bin

OS:bin/OS.bin
	 qemu-system-x86_64.exe -fda bin/OS.bin -boot a

clean:
	rm bin/*
	rm obj/*
run:
	qemu-system-x86_64.exe -fda bin/boot.bin -boot a

com:$(file).c
	gcc -ffreestanding -c $(file).c -o obj/$(file).o

link:$(file).c
	ld -o bin/$(file).bin -e main -Ttext 0x1000 obj/$(file).o --oformat binary	

objdump:$(file).o
	objdump -d -Mintel obj/$(file).o

binary:$(file).c
	gcc -c $(file).c -o bin/$(file).bin

disasm:$(file).bin
	ndisasm -b 32 bin/$(file).bin > dis/$(file).dis

build:	
	nasm -f bin zeros.asm -o bin/zeros.bin	#assembles buffer to make sure reading is done 
	nasm -f bin boot.asm -o bin/boot.bin	#assembles boot loader
	nasm kernel/kernel_entry.asm -f elf32 -o obj/kernel_entry.o	#assembles kernel entry routine
	gcc -fno-pie -ffreestanding -c -m32 kernel/kernel.c -o obj/kernel.o	#compiles kernel.c in 32bit 
	
	ld -o bin/full_kernel.bin -e main -m elf_i386 -s -Ttext 0x1000 obj/kernel_entry.o obj/kernel.o --oformat binary 
	#links object files
	
	cat bin/boot.bin bin/full_kernel.bin bin/zeros.bin > bin/OS.bin		#concatenate binary files into one file
	qemu-system-x86_64.exe -fda bin/OS.bin -boot a	#run the binary using qemu
