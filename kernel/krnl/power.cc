
#include <system.h>
#include <port.h>

namespace power {

void reboot(void) {
	outportb(0x64, 0xfe);
}

} /* power */
