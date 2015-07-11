
#include <stdint.h>

extern int32_t __CTOR_LIST__;
typedef void (* fn_ptr) (void);

static fn_ptr *lpfn_ptr = (fn_ptr *) &__CTOR_LIST__;

void __ctor(void) {
	if (lpfn_ptr == NULL) {
		return; // no ctors
	}

	while (*lpfn_ptr != NULL) {
		(*lpfn_ptr++) ();
	}
}

