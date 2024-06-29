boot.bin:gdt.asm boot.asm src/printf.asm src/print.asm protected_mode.asm
	nasm -f bin boot.asm -o boot.bin	

clear:
	rm boot.bin

run:
	qemu-system-x86_64.exe -fda boot.bin -boot a
