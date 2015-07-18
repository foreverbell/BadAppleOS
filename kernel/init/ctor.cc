
#include <stdint.h>

extern int32_t __INIT_ARRAY_LIST__;
extern int32_t __CTOR_LIST__;

typedef void (* fn_ptr) (void);

static fn_ptr *lpfn_inta_ptr = (fn_ptr *) &__INIT_ARRAY_LIST__;
static fn_ptr *lpfn_ctor_ptr = (fn_ptr *) &__CTOR_LIST__;

static void walk_ctor(fn_ptr *lpfn_ptr) {
	int ctor_count = (int) (*lpfn_ptr);
	
	for (int i = 1; i <= ctor_count; ++i) {
		(*(lpfn_ptr + i))();
	}
}

void __ctor(void) {
	if (lpfn_inta_ptr != NULL) {
		walk_ctor(lpfn_inta_ptr);
	}
	
	if (lpfn_ctor_ptr != NULL) {
		walk_ctor(lpfn_ctor_ptr);
	}
}

