OSIMG = os.img

AS = nasm

all: boot.bin

boot.bin: boot.asm floppy.asm print.asm
	$(AS) -o boot.bin boot.asm
	cp boot.bin ../build/boot.bin

dump:
	ndisasm -b16 -o7C00h boot.bin > dump.txt

clean:
	rm boot.bin

.PHONY: all dump clean