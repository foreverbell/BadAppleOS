
#include <stdio.h>

namespace mm {

namespace {
	
int silent = 0;

}

void logging(int s) {
	silent = s;
}

void printf(const char *fmt, ...) {
	if (!silent) {
	    va_list arp;
	    
	    va_start(arp, fmt);
	    ::printf(fmt, arp);
	    va_end(arp);
	}
}

} /* mm */