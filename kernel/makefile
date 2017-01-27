VIDEO_RAW = ../resource/video.txt

ASFLAGS = -f elf
CFLAGS = -O2 -ffreestanding -fno-builtin -fno-pic -nostdinc -fno-exceptions -fleading-underscore -I./include -I./stl -Wall -Wextra
CCFLAGS = -std=c++11 -fno-rtti $(CFLAGS)
LDFLAGS = -T link.ld -mi386pe
OCFLAGS = -Obinary

AS = nasm
CC = gcc
LD = ld
OC = objcopy

AS_OBJECTS := \
	$(patsubst %.asm, %.o, $(wildcard krnl/*.asm))

CC_OBJECTS := \
	$(patsubst %.cc, %.o, init.cc) \
	$(patsubst %.cc, %.o, $(wildcard ba/*.cc)) \
	$(patsubst %.cc, %.o, $(wildcard krnl/*.cc)) \
	$(patsubst %.cc, %.o, $(wildcard krnl/abi/*.cc)) \
	$(patsubst %.cc, %.o, $(wildcard krnl/cpu/*.cc)) \
	$(patsubst %.cc, %.o, $(wildcard mm/*.cc)) \
	$(patsubst %.cc, %.o, $(wildcard util/*.cc))

all: kernel.bin

kernel.bin: $(AS_OBJECTS) $(CC_OBJECTS) begin.o tmppgdir.o vdata.o
	$(LD) -o kernel.out $(LDFLAGS) $(AS_OBJECTS) $(CC_OBJECTS) tmppgdir.o vdata.o
	$(OC) $(OCFLAGS) kernel.out kernel.bin	
	rm kernel.out
	cp kernel.bin ../build/kernel.bin

begin.o: begin.asm
	$(AS) -o begin.o $(ASFLAGS) begin.asm

tmppgdir.o: tmppgdir.c
	$(CC) -m32 -c -o tmppgdir.o $(CFLAGS) tmppgdir.c

vdata.bin: 
	python ../script/encode.py $(VIDEO_RAW) vdata.bin

vdata.o: vdata.bin
	objcopy -B i386 -I binary -O elf32-i386 vdata.bin vdata.o
	
$(AS_OBJECTS): %.o : %.asm
	$(AS) -o $@ $(ASFLAGS) $<
	
$(CC_OBJECTS): %.o : %.cc
	$(CC) -m32 -c -o $@ $(CCFLAGS) $<
	
dump:
	ndisasm -b32 -oC0000000h kernel.bin > dump.txt

clean:
	rm begin.o vdata.o init.o tmppgdir.o
	rm ba/*.o
	rm krnl/*.o
	rm krnl/abi/*.o
	rm krnl/cpu/*.o
	rm mm/*.o
	rm util/*.o
	rm kernel.bin vdata.bin

.PHONY: all dump clean
