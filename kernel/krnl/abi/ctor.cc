
#include <stdint.h>
#include <stdio.h>
#include <abi.h>

extern int32_t __INIT_ARRAY_LIST__;
extern int32_t __CTOR_LIST__;

typedef void (* fn_ptr) (void);

namespace abi {

namespace {
	
fn_ptr *lpfn_inta_ptr = (fn_ptr *) &__INIT_ARRAY_LIST__;
fn_ptr *lpfn_ctor_ptr = (fn_ptr *) &__CTOR_LIST__;

int wkctor(fn_ptr *lpfn_ptr) {
	if (lpfn_ptr != NULL) {
		int ctor_count = (int) (*lpfn_ptr);
		for (int i = 1; i <= ctor_count; ++i) {
			printf("[ctor] Calling 0x%x.\n", lpfn_ptr);
			(*(lpfn_ptr + i))();
		}
		return ctor_count;
	}
	return 0;
}

}

void ctors(void) {
	int count = 0;
	
	puts("[ctor] Initializing.");
	count += wkctor(lpfn_inta_ptr);
	count += wkctor(lpfn_ctor_ptr);
	printf("Total %d ctors initialized.\n", count);
}

} /* abi */
