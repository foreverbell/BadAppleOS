#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <panic.h>
#include "sbrk.h"

#define fprintf(fd, format, args...) printf(format, ##args)

#ifdef WIN32
#undef WIN32
#endif /* WIN32 */

#ifdef _WIN32
#undef _WIN32
#endif /* _WIN32 */

#define HAVE_MORECORE  1
#define HAVE_MMAP      0
#define USE_LOCKS      0

#define ABORT          panic::panic("Error in dlmalloc " __FILE__, __LINE__)
#define EINVAL	       0
#define ENOMEM 	       0

#define MORECORE       mm::sbrk

#define LACKS_UNISTD_H
#define LACKS_SYS_PARAM_H
#define LACKS_SYS_MMAN_H
#define LACKS_SYS_TYPES_H
#define LACKS_ERRNO_H
#define LACKS_SCHED_H
#define LACKS_TIME_H
#define LACKS_STDIO_H
#define LACKS_STDLIB_H

#ifndef MALLOC_FAILURE_ACTION
#define MALLOC_FAILURE_ACTION
#endif /* MALLOC_FAILURE_ACTION */

#endif