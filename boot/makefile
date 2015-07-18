OSIMG = os.img

AS = nasm

all: boot.bin
	
boot.bin: boot.asm loader.asm print.asm
	$(AS) -o boot.bin boot.asm
	cp boot.bin ../build/boot.bin

clean:
	rm boot.bin

.PHONY: all