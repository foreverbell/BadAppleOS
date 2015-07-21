# Physical Memory Layout

Address                 Memory
---------------------   ------------------------------
1000h ~ 2000h           Page Directory Entry
2000h ~ 6000h           Page Table Entries
7C00h ~ 7E00h           Boot loader
8000h ~ ?????           Store the result of memory detection via `int 15h`
9000h ~ B000h           Middleware (for switching to protected mode), 8K
FFFFh ~ B000h           Kernel stack
10000h ~ 90000h         Kernel, 512K [^1]
100000h ~ 900000h       Kernel memory pool, 8M

[^1]: 80000h ~ 90000h may be not available, See [here](http://wiki.osdev.org/Memory\_Map\_(x86)).

# Virtual Memory Layout in `middle`

PDE is located at phyaddr 1000h ~ 2000h.

Phyaddr of PTE        Mapped phyaddr       Virtaddr                 Size
----------------      -------------------  -----------------------  -------
2000h ~ 3000h [^2]    0h ~ 100000h         0h ~ 100000h             1M
3000h ~ 4000h [^3]    10000h ~ 90000h      C0000000h ~ C0080000h    512K
4000h ~ 5000h [^4]    100000h ~ 500000h    C0400000h ~ C0800000h    4M 
5000h ~ 6000h [^4]    500000h ~ 900000h    C0800000h ~ C0C00000h    4M

[^2]: for identity paging.
[^3]: for kernel.
[^4]: for kernel memory pool.
