#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <panic.h>

#define STRINGIZE(x)     DO_STRINGIZE(x)
#define DO_STRINGIZE(x)  #x

#define assert(expr)    (void)( (!!(expr)) || (panic::panic("Assertion " STRINGIZE(expr) " failed in file " __FILE__ ", line " STRINGIZE(__LINE__), 0), 0) )

#endif
