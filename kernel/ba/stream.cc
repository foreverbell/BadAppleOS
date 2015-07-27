
#include "badapple.h"
#include <panic.h>

namespace badapple {
	
stream::stream(const uint8_t *pool, const uint8_t *pool_end) :
	pool(pool),
	pool_end(pool_end),
	seek(pool),
	shift(0) { }
	
bool stream::has_next() const {
	return seek < pool_end;
}

int stream::nextb() {
	if (!has_next()) {
		panic::panic("[stream] Out of bound.");
	}
	
	int bit = ((*seek) >> shift) & 1;
	shift += 1;
	if (shift == 8) {
		shift = 0;
		seek += 1;
	}
	return bit;
}

int stream::remain() const {
	return (pool_end - seek - 1) * 8 + (8 - shift);
}

} /* badapple */