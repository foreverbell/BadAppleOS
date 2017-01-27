# Memory Layout

## Physical Memory Layout

Address                 Memory
---------------------   ------------------------------
1000h ~ 2000h           Page Directory Entry
2000h ~ 6000h           Page Table Entries
7C00h ~ 7E00h           Boot loader
8000h ~ ?????           Store the result of memory detection via `int 15h`
FFFFh ~ B000h           Kernel stack, 20K [^1]
10000h ~ 90000h         Kernel, 512K [^2]
100000h ~ 900000h       Kernel memory pool, 8M

Table: Physical memory layout

[^1]: Since 9000h ~ B000h is free when executing kernel, so actually kernel stack is 28K.
[^2]: 80000h ~ 90000h may be not available, See [here](http://wiki.osdev.org/Memory\_Map\_(x86)).

## Virtual Memory Layout

Bootstrap PDE and PTE are hardcoded in `tmppgdir.c`.

The whole PDE and PTE are initialized in `kernel/mm/init.cc`.

PDE is located at phyaddr 1000h ~ 2000h. 

Phyaddr of PTE        Mapped phyaddr       Virtaddr                 Size
----------------      -------------------  -----------------------  -------
2000h ~ 3000h         0h ~ 100000h         0h ~ 100000h             1M
3000h ~ 4000h         10000h ~ 90000h      C0000000h ~ C0080000h    512K
4000h ~ 5000h         100000h ~ 500000h    C0400000h ~ C0800000h    4M 
5000h ~ 6000h         500000h ~ 900000h    C0800000h ~ C0C00000h    4M

Table: Location of PTE

The first entry is set to identity paging for some access convenience. The second is for kernel, and the last two are for kernel memory pool.
