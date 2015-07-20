# Physical Memory Layout

* 1000h ~ 2000h: Page Directory Entry
* 2000h ~ 6000h: Page Table Entries
* 7C00h ~ 7E00h: Boot loader
* 8000h ~ ?????: Store the result of memory detection via `int 15h`
* 9000h ~ B000h: Middleware (for switching to protected mode), 8K
* FFFFh ~ B000h: Kernel stack
* 10000h ~ 90000h: Kernel, 512K [^1]
* 100000h ~ 900000h: Memory pool, 8M

[^1]: 80000h ~ 90000h may be not available, See [here](http://wiki.osdev.org/Memory\_Map\_(x86)).

# Virtual Memory Layout

See '/middle/page_init.cc'
