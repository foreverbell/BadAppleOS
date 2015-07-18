OSIMG = os.img

ASFLAGS = -f elf
CCFLAGS = -ffreestanding -fno-builtin -nostdinc -fno-exceptions -fleading-underscore -fno-rtti -Wall -Wextra
LDFLAGS = -T link.ld -mi386pe
OCFLAGS = -Obinary

AS = nasm
CC = g++
LD = ld
OC = objcopy

all: middle.bin

middle.bin: begin.o gdt.o memory.o
	$(LD) -o middle.out $(LDFLAGS) gdt.o memory.o
	$(OC) $(OCFLAGS) middle.out middle.bin
	rm middle.out
	python ../padding.py middle.bin 8192
	cp middle.bin ../build/middle.bin

begin.o: begin.asm
	$(AS) -o begin.o $(ASFLAGS) begin.asm

gdt.o: gdt.asm
	$(AS) -o gdt.o $(ASFLAGS) gdt.asm
	
memory.o: memory.cc
	$(CC) -m32 -c -o memory.o $(CCFLAGS) memory.cc

clean:
	rm *.o
	rm middle.bin
	
.PHONY: all clean