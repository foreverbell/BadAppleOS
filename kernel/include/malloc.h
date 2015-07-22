#ifndef __MALLOC_H__
#define __MALLOC_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void *malloc(size_t);
void free(void *);
void *calloc(size_t, size_t);
void *realloc(void *, size_t);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif