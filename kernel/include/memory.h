#ifndef __MEMORY_H__
#define __MEMORY_H__

namespace mm {

void initialize(void);

#define MM_LOG_SILENT  1
#define MM_LOG_NOISY   0

void logging(int);
void printf(const char *, ...);

} /* mm */

#endif
