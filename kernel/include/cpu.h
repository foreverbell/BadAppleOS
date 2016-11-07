#ifndef __CPU_H__
#define __CPU_H__

#include "system.h"

namespace cpu {

void initialize(void);

namespace manipulate {

void cli(void);
void sti(void);
void halt(void);
void die(void);
void sleep(uint64_t);

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

} /* manipulate */

namespace cpuid {

bool support();
void vendor(char *);
void feature(int *, int *);

} /* cpuid */

namespace power {

void reboot(void);

} /* power */

} /* cpu */

#endif
