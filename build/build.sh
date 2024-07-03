#!/bin/bash
#not full script to run the operating system

nasm kernel/kernel_entry.asm -f elf32 -o obj/kernel_entry.o
gcc -fno-pie -ffreestanding -c -m32 kernel/kernel.c -o obj/kernel.o
ld -o bin/full_kernel.bin -e main -m elf_i386 -s -Ttext 0x1000 obj/kernel_entry.o obj/kernel.o --oformat binary
cat bin/boot.bin bin/full_kernel.bin > bin/OS.bin
qemu-system-x86_64.exe -fda bin/OS.bin -boot a
