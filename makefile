#TODO ==> add a function to loop though all the object files needed and builds them


all:OS	

file=kernel
KERNEL_FILES := $(wildcard kernel/*.asm)
KERNEL_HEADER := $(wildcard kernel/src/*.h)

BOOT_FILE := boot/boot.asm
ZERO_FILE := boot/zeros.asm

#BOOT_BIN needs all in gdt , src , kernel
BOOT_BIN := $(wildcard boot/gdt/*.asm boot/src/*.asm)


#/////////////////////////#
#     The BOOT files     #
#/////////////////////////#
bin/boot.bin:$(BOOT_BIN) $(KERNEL_FILES)
	nasm -f bin $(BOOT_FILE) -o bin/boot.bin	

bin/zeros.bin:$(ZERO_FILE)
	nasm -f bin $(ZERO_FILE) -o bin/zeros.bin

#/////////////////////////#
#    The object files     #
#/////////////////////////#

obj/kernel_entry.o:$(KERNEL_FILES)
	nasm kernel/kernel_entry.asm -f elf32 -o obj/kernel_entry.o

#probebly where us should add if u added files to kernel
obj/kernel.o:kernel/kernel.c $(KERNEL_HEADER) 
	gcc -Wall -Wextra -Werror -fno-pie -ffreestanding -m32 -c kernel/kernel.c -o obj/kernel.o 
# i dont think ill need to refactor much code here 
obj/kernel_io.o:kernel/src/io.s
	@$(call CREATE_OBJ_S,"kernel/src/io.s","obj/kernel_io.o")
obj/kernel_page.o:kernel/src/page.s
	@$(call CREATE_OBJ_S,"kernel/src/page.s","obj/kernel_page.o")
obj/kernel_idt.o:kernel/src/idt.s
	@$(call CREATE_OBJ_S,"kernel/src/idt.s","obj/kernel_idt.o")

# ADD THE NEW OBJECT FILE HERE AND ADD A METHODE TO BUILD THEM
KERNEL_OBJECT_FILES := obj/kernel_entry.o obj/kernel.o obj/kernel_io.o obj/kernel_page.o obj/kernel_idt.o 
#/////////////////////////#
#    The binary files     #
#/////////////////////////#

bin/full_kernel.bin:$(KERNEL_OBJECT_FILES) 
	ld -o bin/full_kernel.bin -e main -m elf_i386 -s -Ttext 0x1000 $(KERNEL_OBJECT_FILES) --oformat binary

bin/OS.bin:bin/boot.bin bin/full_kernel.bin bin/zeros.bin
	cat bin/boot.bin bin/full_kernel.bin bin/zeros.bin > bin/OS.bin

OS:bin/OS.bin
	 qemu-system-x86_64.exe -fda bin/OS.bin -boot a

#/////////////////////////#
#       The  TOOLS        #
#/////////////////////////#

# This is a function that transforms assembly files to objects to be integrated to the kernel binary file
define CREATE_OBJ_S
	$(eval SFILE = $(1))
	$(eval OFILE = $(2))
	echo "Compiling $(SFILE) to $(OFILE) ..."
	gcc -c -m32 -masm=intel -Wall -Wextra $(SFILE) -o $(OFILE) 
endef

clean:
	rm bin/*
	rm obj/*
run:
	qemu-system-x86_64.exe -fda bin/boot.bin -boot a

#the block bellow prolly disassembles bin for check ups and exprerimentation
com:$(file).c
	gcc -Wall -Wextra -Werror -ffreestanding -c $(file).c -o obj/$(file).o

link:$(file).c
	ld -o bin/$(file).bin -e main -Ttext 0x1000 obj/$(file).o --oformat binary	

objdump:$(file).o
	objdump -d -Mintel obj/$(file).o

binary:$(file).c
	gcc -Wall -Wextra -Werror -c $(file).c -o bin/$(file).bin

disasm:$(file).bin
	ndisasm -b 32 bin/$(file).bin > dis/$(file).dis

build:	
	nasm -f bin $(BOOT_FILE) -o bin/zeros.bin																		#assembles buffer to make sure reading is done 
	nasm -f bin $(BOOT_FILE) -o bin/boot.bin																		#assembles boot loader
	nasm kernel/kernel_entry.asm -f elf32 -o obj/kernel_entry.o														#assembles kernel entry routine
	gcc -Wall -Wextra -Werror -fno-pie -ffreestanding -c -m32 kernel/kernel.c -o obj/kernel.o												#compiles kernel.c in 32bit 
	
	ld -o bin/full_kernel.bin -e main -m elf_i386 -s -Ttext 0x1000 obj/kernel_entry.o obj/kernel.o --oformat binary #links object files
	
	cat bin/boot.bin bin/full_kernel.bin bin/zeros.bin > bin/OS.bin													#concatenate binary files into one file
	qemu-system-x86_64.exe -fda bin/OS.bin -boot a																	#run the binary using qemu

