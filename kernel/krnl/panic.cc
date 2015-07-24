
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
	if (what != NULL) {
		printf("\tMessage: %s", what);
	}
	printf("\tCode: %d\n", code);
	
	cpu::die();
}

void panic(const char *what) {
	panic(what, 0);
}

} /* panic */
