file=kernel

bin/boot.bin:gdt/gdt.asm boot.asm src/printf.asm src/printf32.asm gdt/load_gdt.asm kernel/load_kernel.asm src/loaddisk.asm 
	nasm -f bin boot.asm -o bin/boot.bin	

bin/zeros.bin:zeros.asm
	nasm -f bin zeros.asm -o bin/zeros.bin

obj/kernel_entry.o:kernel/kernel_entry.asm
	nasm kernel/kernel_entry.asm -f elf32 -o obj/kernel_entry.o

obj/kernel.o:kernel/kernel.c
	 gcc -fno-pie -ffreestanding -c -m32 kernel/kernel.c -o obj/kernel.o

bin/full_kernel.bin:obj/kernel.o obj/kernel_entry.o
	ld -o bin/full_kernel.bin -e main -m elf_i386 -s -Ttext 0x1000 obj/kernel_entry.o obj/kernel.o --oformat binary

bin/OS.bin:bin/boot.bin bin/full_kernel.bin bin/zeros.bin
	cat bin/boot.bin bin/full_kernel.bin bin/zeros.bin > bin/OS.bin

start:
	 qemu-system-x86_64.exe -fda bin/OS.bin -boot a

clear:
	rm bin/boot.bin

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

os:	
	nasm -f bin zeros.asm -o bin/zeros.bin	#assembles buffer to make sure reading is done 
	nasm -f bin boot.asm -o bin/boot.bin	#assembles boot loader
	nasm kernel/kernel_entry.asm -f elf32 -o obj/kernel_entry.o	#assembles kernel entry routine
	gcc -fno-pie -ffreestanding -c -m32 kernel/kernel.c -o obj/kernel.o	#compiles kernel.c in 32bit 
	
	ld -o bin/full_kernel.bin -e main -m elf_i386 -s -Ttext 0x1000 obj/kernel_entry.o obj/kernel.o --oformat binary	#links object files
	cat bin/boot.bin bin/full_kernel.bin bin/zeros.bin > bin/OS.bin		#concatenate binary files into one file
	qemu-system-x86_64.exe -fda bin/OS.bin -boot a	#run the binary using qemu
