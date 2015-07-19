# BadAppleOS

(终于完成了我多年的一个心愿..)

# Description

Displaying [BadApple](https://www.youtube.com/watch?v=VzEUeWnV73U) animation in the protected mode of a operating system.

This operating system is quite rough, even it doesn't have a memory manager, task scheduler and other more advance things.

# Motivation

At early year of 2011, I was inspired by some of my friends who were doing the similar things. Simultaneously I was learning something related to operating system, so I wrote something ugly which did the same thing as this repository, code still can be found [here](http://pan.baidu.com/s/1dDHEpwt).

In the summer of 2014, I have some spare time, so I decide to relearn something of operating system. As a consequence, I wrote some C++ code, but due to some reasons it had to be stopped. However, I realized that those rough code is sufficent for me to rewrite a BadAppleOS.

# Highlight

* Mixing 16bit Assembly and C++ code in the middleware `middle` (switching to protected mode).
* Except some necessary booting code written in Assembly, other code is purely written in C++. I write some black magic to make some features of C++ like class and constructor work in my OS.

# Toolchains

It is developed under Windows, though I think it would also work under Linux (Actually I have tested it under Ubuntu 14.0.2).

The toolchains follow here, 

* gcc (4.9.2, x86\_64-pc-msys)
* nasm (2.11.08)
* python2

I don't guarantee other versions of toolchains will work.

# Usage

A runnable build can be found [here](https://github.com/foreverbell/miscellaneous/raw/master/resource/BadAppleOS/os.img).

To build, see `makefile`, then create a new VMware virtual machine (or other virtual machines) with the build `os.img` as floppy image.

You can also burn it on your USB, and enjoy the BadApple on your real computer :)
