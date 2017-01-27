OSIMG = build/os.img

all: boot kernel
	python script/link.py build/boot.bin build/kernel.bin $(OSIMG)

boot: 
	cd boot && make && cd ..
	
kernel: 
	cd kernel && make && cd ..
	
run: $(OSIMG)
	bochs -q -f bochsrc

debug: $(OSIMG)
	bochsdbg -q -f bochsrc

qemu: $(OSIMG)
	qemu-system-i386 -drive file=build/os.img,index=0,media=disk,format=raw

dump:
	cd boot && make dump && cd ..
	cd kernel && make dump && cd ..
	
clean:
	cd boot && make clean && cd ..
	cd kernel && make clean && cd ..
	rm build/*.bin
	rm $(OSIMG)

.PHONY: all boot kernel run debug qemu dump clean
