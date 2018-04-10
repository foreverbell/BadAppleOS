AS := nasm

all: boot.bin

build:
	mkdir -p ../build

boot.bin: build boot.asm disk.asm print.asm
	$(AS) -o boot.bin boot.asm
	cp boot.bin ../build/boot.bin

dump:
	ndisasm -b16 -o7C00h boot.bin > dump.txt

clean:
	rm -f boot.bin

.PHONY: all dump clean
