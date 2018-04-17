OSIMG := build/os.img
OSISO_GRUB := build/os_grub.iso

all: img qemu

# necessary if we want to build `grub_iso`.
docker:
	docker build -t=grub:latest docker

img: boot kernel
	python link.py build/boot.bin build/kernel.bin $(OSIMG)

grub_iso: kernel
	mkdir -p build/iso/boot/grub
	cp grub.cfg build/iso/boot/grub/grub.cfg
	cp build/kernel.elf build/iso/boot/kernel.elf
	docker run -it --rm -v $(shell pwd):$(shell pwd) -w $(shell pwd) -u `id -u $(shell USER)` grub:latest -o $(OSISO_GRUB) build/iso

boot:
	cd boot && make

kernel:
	cd kernel && make

qemu: $(OSIMG)
	qemu-system-i386 -drive file=$(OSIMG),index=0,media=disk,format=raw

qemu_grub: $(OSISO_GRUB)
	qemu-system-i386 -cdrom $(OSISO_GRUB)

clean:
	cd boot && make clean
	cd kernel && make clean
	rm -rf build

.PHONY: docker img grub_iso boot kernel qemu qemu_grub clean
