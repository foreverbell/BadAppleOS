#ifndef __CPU_MANIPULATE_H__
#define __CPU_MANIPULATE_H__

#include "system.h"

namespace cpu {
	
void initialize(void);
void cli(void);
void sti(void);
void halt(void);
void die(void);

/* Interruption guard (scoped/RAII). */
class int_guard {
public:
	int_guard();
	~int_guard();
	
	void acquire(void); /* clear interruption. */
	void release(void); /* set interruption. */
	bool owns(void) const; /* if already cleared interruption. */
	
private:
	bool cleared;
};

} /* cpu */

#endif
