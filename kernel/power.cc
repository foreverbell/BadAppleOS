
#include <system.h>
#include <stdio.h>
#include <stdint.h>

namespace power {

void reboot(void) {
	outportb(0x64, 0xfe);
}

}