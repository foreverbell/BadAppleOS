OSIMG = os.img

ASFLAGS = -f elf
CCFLAGS = -ffreestanding -fno-builtin -fno-pic -nostdinc -fno-exceptions -fleading-underscore -fno-rtti -fno-toplevel-reorder -Wall -Wextra
LDFLAGS = -T link.ld -mi386pe
OCFLAGS = -Obinary

AS = nasm
CC = g++
LD = ld
OC = objcopy

all: middle.bin

middle.bin: begin.o gdt.o mem.o mem_detect.o print.o page_init.o
	$(LD) -o middle.out $(LDFLAGS) gdt.o mem.o mem_detect.o print.o page_init.o
	$(OC) $(OCFLAGS) middle.out middle.bin
	rm middle.out
	cp middle.bin ../build/middle.bin

begin.o: begin.asm
	$(AS) -o begin.o $(ASFLAGS) begin.asm

gdt.o: gdt.asm
	$(AS) -o gdt.o $(ASFLAGS) gdt.asm

print.o: print.asm
	$(AS) -o print.o $(ASFLAGS) print.asm
	
mem.o: mem.asm
	$(AS) -o mem.o $(ASFLAGS) mem.asm

mem_detect.o: mem_detect.cc
	$(CC) -m32 -c -o mem_detect.o $(CCFLAGS) mem_detect.cc

page_init.o: page_init.cc
	$(CC) -m32 -c -o page_init.o $(CCFLAGS) page_init.cc

dump:
	ndisasm -b16 -o9000h middle.bin > dump.txt

clean:
	rm *.o
	rm middle.bin
	
.PHONY: all dump clean
