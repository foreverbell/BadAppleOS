OSIMG = build/os.img

all: boot middle kernel
	python script/link.py build/boot.bin build/middle.bin build/kernel.bin $(OSIMG)

boot: 
	cd boot && make && cd ..
	
middle: 
	cd middle && make && cd ..
	
kernel: 
	cd kernel && make && cd ..
	
run: $(OSIMG)
	bochs -q -f bochsrc
	
debug: $(OSIMG)
	bochsdbg -q -f bochsrc
	
dump:
	cd boot && make dump && cd ..
	cd middle && make dump && cd ..
	cd kernel && make dump && cd ..
	
clean:
	cd boot && make clean && cd ..
	cd middle && make clean && cd ..
	cd kernel && make clean && cd ..
	rm build/*.bin
	rm $(OSIMG)

.PHONY: all boot middle kernel run clean dump
