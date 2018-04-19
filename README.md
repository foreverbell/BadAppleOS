# BadAppleOS

# Description

Displaying [BadApple](https://www.youtube.com/watch?v=VzEUeWnV73U) animation in the protected mode of an x86 operating system.

This operating system is quite rough, even it doesn't have a file system, task scheduler and other more advanced things.

So it is not an operating system indeed, just a program running on a bare computer LOL :)

# Highlight

* Except some necessary booting code written in Assembly and C, other code is purely written in C++(11). 
* A minimal STL port.
* Artify the plain text to a great ASCII Art. 
* ... and this operating system is only 168K (depends on your compiler)!

Notice you may see some weird designs in this OS, this is because one of our preliminary goal is to make it fit into a floppy.

# Toolchains

Tested under Ubuntu xenial with the following toolchains,

* gcc (5.4.0)
* nasm (2.11.08)
* python (2.7.12)

I don't guarantee other versions of toolchains will work.

# Build

BadAppleOS is multi-boot compliant, so you can boot it with GRUB. We also provide a minimal bootloader without GRUB.

## No GRUB

```bash
make img
```

Run `make qemu` to load the OS under QEMU.

## GRUB

```bash
make docker
make grub_iso
```

The docker step is sometimes necessary, because `grub-mkrescue` may not work properly under some systems.

Run `make qemu_iso` to load the OS under QEMU.

# License 

MIT
