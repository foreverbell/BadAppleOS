#ifndef __SYSTIME_H__
#define __SYSTIME_H__

namespace systime {

struct systime_t {
	int year, month, day;
	int hour, minute, second;
	friend bool operator == (const systime_t &, const systime_t &);
	friend bool operator != (const systime_t &, const systime_t &);
	systime_t();
}; 

systime_t *get_systime(systime_t *);

} /* systime */ 

#endif
