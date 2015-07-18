OSIMG = build/os.img

all: $(OSIMG)

$(OSIMG): build/boot.bin build/middle.bin build/kernel.bin
	cat build/boot.bin build/middle.bin build/kernel.bin > $(OSIMG)

build/boot.bin:
	cd boot && make && cd ..
	
build/middle.bin:
	cd middle && make && cd ..

build/kernel.bin:
	cd kernel && make && cd ..

run: $(OSIMG)
	bochs -q -f bochsrc
	
debug: $(OSIMG)
	bochsdbg -q -f bochsrc
	
dump:
	ndisasm -b32 -o9000h build/kernel.bin > build/dump.txt
	
clean:
	cd boot && make clean && cd ..
	cd middle && make clean && cd ..
	cd kernel && make clean && cd ..
	rm build/*.bin
	rm $(OSIMG)

.PHONY: all run clean dump