
#include <system.h>
#include <stdio.h>
#include <stdint.h>
#include <console.h>

using console::setcolor;
using console::mkcolor;
using namespace console::vga_color;

namespace panic {

void panic(const char *what, int code) {
	setcolor(mkcolor(red, black), false);
	
	printf("Kernel panic!\n");
	printf("\tMessage: %s\n\tCode: %d\n", what, code);
	
	__asm__ __volatile__ ("cli; hlt");
}

} /* panic */