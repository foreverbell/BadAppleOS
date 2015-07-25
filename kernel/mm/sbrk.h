#ifndef __SBRK_H__
#define __SBRK_H__

#include <stdint.h>

namespace mm {
	
void *sbrk(ptrdiff_t);

} /* mm */

#endif