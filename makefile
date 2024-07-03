bin/boot.bin:gdt.asm boot.asm src/printf.asm src/printf32.asm load_gdt.asm load_kernel.asm src/loaddisk.asm 
	nasm -f bin boot.asm -o bin/boot.bin	

clear:
	rm bin/boot.bin

run:
	qemu-system-x86_64.exe -fda bin/boot.bin -boot a

file=kernel


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
	i386-elf-gcc -ffreestanding -m32 -g -c "kernel.c" -o "obj/kernel.o"
	nasm "kernel_entry.asm" -f elf -o "obj/kernel_entry.o"
	i386-elf-ld -o "bin/full_kernel.bin" -Ttext 0x1000 "obj/kernel_entry.o" "obj/kernel.o" --oformat binary
	nasm -f bin boot.asm -o bin/boot.bin
	nasm -f bin zeros.asm -o bin/zeros.bin
	cat "bin/boot.bin" "bin/full_kernel.bin" > "bin/ready.bin"
	cat "bin/ready.bin" "bin/zeros.bin" > "bin/OS.bin"
	qemu-system-x86_64.exe -drive format=raw,file="bin/OS.bin",index=0,if=floppy, -m 128M 
