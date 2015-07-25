
#include <system.h>
#include <port.h>

namespace power {

void reboot(void) {
	port::outb(0x64, 0xfe);
}

} /* power */
