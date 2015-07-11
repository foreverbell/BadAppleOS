OSIMG = os.img
VIDEO_RAW = video.txt

ASFLAGS = -f elf
CCFLAGS = -ffreestanding -fno-builtin -nostdinc -fno-exceptions -fleading-underscore -fno-rtti -I./include -Wall -Wextra
LDFLAGS = -T link.ld -mi386pe
OCFLAGS = -Obinary

AS = nasm
CC = g++
LD = ld
OC = objcopy

AS_OBJECTS := \
	$(patsubst %.asm, %.o, $(wildcard init/*.asm)) \
	$(patsubst %.asm, %.o, $(wildcard kernel/*.asm))

CC_OBJECTS := \
	$(patsubst %.cc, %.o, $(wildcard ba/*.cc)) \
	$(patsubst %.cc, %.o, $(wildcard init/*.cc)) \
	$(patsubst %.cc, %.o, $(wildcard kernel/*.cc)) \
	$(patsubst %.cc, %.o, $(wildcard mm/*.cc)) \
	$(patsubst %.cc, %.o, $(wildcard util/*.cc))

all: $(OSIMG)

$(OSIMG): boot.bin kernel.bin
	cat boot.bin kernel.bin > $(OSIMG)
	python padding.py $(OSIMG)
	
boot.bin: boot/boot.asm
	$(AS) -o boot.bin boot/boot.asm
	
kernel.bin: $(AS_OBJECTS) $(CC_OBJECTS) vdata.o
	$(LD) -o kernel.out $(LDFLAGS) $(AS_OBJECTS) $(CC_OBJECTS) vdata.o
	$(OC) $(OCFLAGS) kernel.out kernel.bin	
	rm kernel.out
	
vdata.bin: 
	python encode.py $(VIDEO_RAW) vdata.bin

vdata.o: vdata.bin
	objcopy -B i386 -I binary -O elf32-i386 vdata.bin vdata.o
	
$(AS_OBJECTS): %.o : %.asm
	$(AS) -o $@ $(ASFLAGS) $<
	
$(CC_OBJECTS): %.o : %.cc
	$(CC) -m32 -c -o $@ $(CCFLAGS) $<
	
run: $(OSIMG)
	bochs -q -f bochsrc
	
debug: $(OSIMG)
	bochsdbg -q -f bochsrc
	
clean:
	rm ba/*.o
	rm init/*.o
	rm kernel/*.o
	rm mm/*.o
	rm util/*.o
	rm vdata.o
	rm boot.bin kernel.bin vdata.bin $(OSIMG)
	rm dump.txt
	
dump:
	ndisasm -b32 -o9000h kernel.bin > dump.txt
	
.PHONY: all run clean dump