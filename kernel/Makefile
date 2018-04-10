ASFLAGS := -f elf
CFLAGS := -O2 -ffreestanding -fno-builtin -fno-pic -nostdinc -fno-exceptions -fleading-underscore -I./include -I./stl -Wall -Wextra
CCFLAGS := -std=c++11 -fno-rtti $(CFLAGS)
LDFLAGS := -T link.ld -mi386pe
OCFLAGS := -Obinary

AS := nasm
CC := gcc
LD := ld
OC := objcopy

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

build:
	mkdir -p ../build

kernel.bin: build $(AS_OBJECTS) $(CC_OBJECTS) begin.o tmppgdir.o vdata.o
	$(LD) -o kernel.out $(LDFLAGS) $(AS_OBJECTS) $(CC_OBJECTS) tmppgdir.o vdata.o
	$(OC) $(OCFLAGS) kernel.out kernel.bin	
	rm -f kernel.out
	cp kernel.bin ../build/kernel.bin

begin.o: begin.asm
	$(AS) -o begin.o $(ASFLAGS) begin.asm

tmppgdir.o: tmppgdir.c
	$(CC) -m32 -c -o tmppgdir.o $(CFLAGS) tmppgdir.c

vdata.bin: 
	python ../resource/encode.py ../resource/video.txt vdata.bin

vdata.o: vdata.bin
	objcopy -B i386 -I binary -O elf32-i386 vdata.bin vdata.o
	
$(AS_OBJECTS): %.o : %.asm
	$(AS) -o $@ $(ASFLAGS) $<
	
$(CC_OBJECTS): %.o : %.cc
	$(CC) -m32 -c -o $@ $(CCFLAGS) $<
	
dump:
	ndisasm -b32 -oC0000000h kernel.bin > dump.txt

clean:
	rm -f *.o **/*.o **/**/*.o
	rm -f vdata.bin kernel.bin

.PHONY: all dump clean
