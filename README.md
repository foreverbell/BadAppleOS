# BadAppleOS

# Description

Displaying [BadApple](https://www.youtube.com/watch?v=VzEUeWnV73U) animation in the protected mode of a x86 operating system.

This operating system is quite rough, even it doesn't have a file system, task scheduler and other more advance things.

So it is not an operating system indeed, just a program running on a bare computer lol :)

# Motivation

At early year of 2010, I was inspired by some of my friends who were doing the similar things. Simultaneously I was learning something related to operating system, so I wrote something ugly which did the same thing as this repository, code still can be found [here](http://pan.baidu.com/s/1dDHEpwt).

In the summer of 2014, I have some spare time, so I decide to relearn something of operating system. As a consequence, I wrote some C++ code, but due to some reasons it had to be stopped. However, I realized that those rough code is sufficent for me to rewrite a BadAppleOS.

# Highlight

* Except some necessary booting code written in Assembly and C, other code is purely written in C++(11). 
* A minimal STL port.
* Artify the plain text to a great ASCII Art. 
* ... and this operating system is only 180K (depends on your compiler)!

# Toolchains

Tested under Ubuntu 16.10 (yakkety).

The toolchains follow here, 

* gcc (6.2.0)
* nasm (2.12.01)
* python (2.7)

I don't guarantee other versions of toolchains will work.

# Usage

A runnable build can be found [here](https://github.com/foreverbell/miscellaneous/raw/master/resource/BadAppleOS/os.img).

To build, see `makefile`, then create a new VMware virtual machine (or other virtual machines) with the build `os.img` as floppy image.

You can also burn it on your USB, and enjoy the BadApple on your real computer :)

# TODO

Multiboot header support to make it compatible with grub.

# License 

MIT
