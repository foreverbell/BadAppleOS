
#include <stdint.h>
#include <abi.h>

extern int32_t __INIT_ARRAY_LIST__;
extern int32_t __CTOR_LIST__;

namespace abi {

typedef void (* fn_ptr) (void);

static fn_ptr *lpfn_inta_ptr = (fn_ptr *) &__INIT_ARRAY_LIST__;
static fn_ptr *lpfn_ctor_ptr = (fn_ptr *) &__CTOR_LIST__;

static void wkctor(fn_ptr *lpfn_ptr) {
	int ctor_count = (int) (*lpfn_ptr);
	
	for (int i = 1; i <= ctor_count; ++i) {
		(*(lpfn_ptr + i))();
	}
}

void ctors(void) {
	if (lpfn_inta_ptr != NULL) {
		wkctor(lpfn_inta_ptr);
	}
	
	if (lpfn_ctor_ptr != NULL) {
		wkctor(lpfn_ctor_ptr);
	}
}

} /* abi */
