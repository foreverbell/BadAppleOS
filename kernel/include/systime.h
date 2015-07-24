#ifndef __SYSTIME_H__
#define __SYSTIME_H__

namespace systime {

struct systime_t {
	int year, month, day;
	int hour, minute, second;
	
	systime_t() {
		year = 2012, month = 12, day = 21;
		hour = 0xd, minute = 0xe, second = 0xa; // where is 0xd ?
	}
	
	friend bool operator == (const systime_t &, const systime_t &);
	friend bool operator != (const systime_t &, const systime_t &);
}; 

inline bool operator == (const systime_t &a, const systime_t &b) {
	return a.second == b.second && 
	  a.minute == b.minute && 
	  a.hour == b.hour &&
	  a.day == b.day &&
	  a.month == b.month &&
	  a.year == b.year;
}

inline bool operator != (const systime_t &a, const systime_t &b) {
	return a.second != b.second ||
	  a.minute != b.minute ||
	  a.hour != b.hour ||
	  a.day != b.day ||
	  a.month != b.month ||
	  a.year != b.year;
}


systime_t *get_systime(systime_t *);

} /* systime */ 

#endif
