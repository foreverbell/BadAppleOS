OSIMG := build/os.img

all: boot kernel
	python link.py build/boot.bin build/kernel.bin $(OSIMG)

boot: 
	cd boot && make
	
kernel: 
	cd kernel && make
	
run: $(OSIMG)
	bochs -q -f bochsrc

debug: $(OSIMG)
	bochsdbg -q -f bochsrc

qemu: $(OSIMG)
	qemu-system-i386 -drive file=build/os.img,index=0,media=disk,format=raw

dump:
	cd boot && make dump
	cd kernel && make dump
	
clean:
	cd boot && make clean
	cd kernel && make clean
	rm -rf build

.PHONY: all boot kernel run debug qemu dump clean
