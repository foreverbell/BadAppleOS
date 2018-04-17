# BadAppleOS

# Description

Displaying [BadApple](https://www.youtube.com/watch?v=VzEUeWnV73U) animation in the protected mode of a x86 operating system.

This operating system is quite rough, even it doesn't have a file system, task scheduler and other more advance things.

So it is not an operating system indeed, just a program running on a bare computer lol :)

# Highlight

* Except some necessary booting code written in Assembly and C, other code is purely written in C++(11). 
* A minimal STL port.
* Artify the plain text to a great ASCII Art. 
* ... and this operating system is only 180K (depends on your compiler)!

# Toolchains

Tested under Ubuntu xenial with the following toolchains,

* gcc (5.4.0)
* nasm (2.11.08)
* python (2.7.12)

I don't guarantee other versions of toolchains will work.

# Build

## No Grub

```bash
make img
```

Run `make qemu` to load the OS under QEMU.

## Grub

```bash
make docker
make grub_iso
```

The docker step is sometimes necessary, because `grub-mkrescue` may not work properly under some systems.

Run `make qemu_iso` to load the OS under QEMU.

# License 

MIT
