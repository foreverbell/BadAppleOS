VIDEO_RAW = ../video.txt

ASFLAGS = -f elf
CCFLAGS = -ffreestanding -fno-builtin -nostdinc -fno-exceptions -fleading-underscore -fno-rtti -I./include -Wall -Wextra
LDFLAGS = -T link.ld -mi386pe
OCFLAGS = -Obinary

AS = nasm
CC = g++
LD = ld
OC = objcopy

AS_OBJECTS := \
	$(patsubst %.asm, %.o, $(wildcard krnl/*.asm))

CC_OBJECTS := \
	$(patsubst %.cc, %.o, init.cc) \
	$(patsubst %.cc, %.o, $(wildcard ba/*.cc)) \
	$(patsubst %.cc, %.o, $(wildcard krnl/*.cc)) \
	$(patsubst %.cc, %.o, $(wildcard mm/*.cc)) \
	$(patsubst %.cc, %.o, $(wildcard util/*.cc))

all: kernel.bin

kernel.bin: $(AS_OBJECTS) $(CC_OBJECTS) begin.o vdata.o
	$(LD) -o kernel.out $(LDFLAGS) $(AS_OBJECTS) $(CC_OBJECTS) vdata.o
	$(OC) $(OCFLAGS) kernel.out kernel.bin	
	rm kernel.out
	python ../padding.py kernel.bin 524288
	cp kernel.bin ../build/kernel.bin

begin.o: begin.asm
	$(AS) -o begin.o $(ASFLAGS) begin.asm
	
vdata.bin: 
	python encode.py $(VIDEO_RAW) vdata.bin

vdata.o: vdata.bin
	objcopy -B i386 -I binary -O elf32-i386 vdata.bin vdata.o
	
$(AS_OBJECTS): %.o : %.asm
	$(AS) -o $@ $(ASFLAGS) $<
	
$(CC_OBJECTS): %.o : %.cc
	$(CC) -m32 -c -std=c++11 -o $@ $(CCFLAGS) $<
	
dump:
	ndisasm -b32 -oC0000000h kernel.bin > dump.txt

clean:
	rm begin.o vdata.o
	rm krnl/*.o
	rm mm/*.o
	rm util/*.o
	rm kernel.bin vdata.bin

.PHONY: all dump clean