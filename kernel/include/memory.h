#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>

namespace mm {

void initialize(void);

#define MM_LOG_SILENT  1
#define MM_LOG_NOISY   0

void log_status(int);
void log_new(size_t, void *);
void log_delete(void *);

} /* mm */

#endif
