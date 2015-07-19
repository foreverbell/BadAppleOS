OSIMG = build/os.img

all: 
	cd boot && make && cd ..
	cd middle && make && cd ..
	cd kernel && make && cd ..
	cat build/boot.bin build/middle.bin build/kernel.bin > $(OSIMG)

run: $(OSIMG)
	bochs -q -f bochsrc
	
debug: $(OSIMG)
	bochsdbg -q -f bochsrc
	
dump:
	ndisasm -b16 -o9000h middle/middle.bin > middle/dump.txt
	ndisasm -b32 -oC0000000h kernel/kernel.bin > kernel/dump.txt

clean:
	cd boot && make clean && cd ..
	cd middle && make clean && cd ..
	cd kernel && make clean && cd ..
	rm build/*.bin
	rm $(OSIMG)

.PHONY: all run clean dump
