
#include <system.h>
#include <port.h>
#include <systime.h>

namespace systime {

const int cmos_addr = 0x70;
const int cmos_data = 0x71;

bool operator == (const systime_t &a, const systime_t &b) {
	return a.second == b.second && 
	  a.minute == b.minute && 
	  a.hour == b.hour &&
	  a.day == b.day &&
	  a.month == b.month &&
	  a.year == b.year;
}

bool operator != (const systime_t &a, const systime_t &b) {
	return a.second != b.second ||
	  a.minute != b.minute ||
	  a.hour != b.hour ||
	  a.day != b.day ||
	  a.month != b.month ||
	  a.year != b.year;
}

systime_t::systime_t() {
	year = 2012, month = 12, day = 21;
	hour = 0xd, minute = 0xe, second = 0xa; // where is 0xd ?
}

static systime_t systime;

static int read_cmos(int reg) {
#ifdef NMI_DISABLE
	reg |= (1 << 7);
#endif
	outportb(cmos_addr, reg);
	return inportb(cmos_data);
}

static bool update_in_progress() {
	int flag = read_cmos(0xa) & 0x80;
	return (flag ? true : false);
}

static void fill_systime(systime_t *in_ptr) {
	while (update_in_progress());
	in_ptr->second = read_cmos(0x0);
	in_ptr->minute = read_cmos(0x2);
	in_ptr->hour = read_cmos(0x4);
	in_ptr->day = read_cmos(0x7);
	in_ptr->month = read_cmos(0x8);
	in_ptr->year = read_cmos(0x9);
}

systime_t *get_systime(systime_t *in_ptr) {
	systime_t cur_systime;

	for (fill_systime(&systime); ; ) {
		fill_systime(&cur_systime);
		if (cur_systime == systime) {
			break;
		}
		systime = cur_systime;
	}

	/* read registerB. */
	int regB = read_cmos(0xb);

	/* read and clear 0x80 bit of hour. */
	int hour_h8 = systime.hour & 0x80;
	systime.hour &= ~0x80;

	if (~regB & 0x4) { 
		/* detected BCD mode, convert to binary mode. */
#define BCD_TO_BINART(x) (((x) & 0xf) + (((x) >> 4) * 10))
		systime.second = BCD_TO_BINART(systime.second);
		systime.minute = BCD_TO_BINART(systime.minute);
		systime.hour = BCD_TO_BINART(systime.hour);
		systime.day = BCD_TO_BINART(systime.day);
		systime.month = BCD_TO_BINART(systime.month);
		systime.year = BCD_TO_BINART(systime.year);
#undef BCD_TO_BINART
	}

	/* convert 12 hour to 24 hour if necessary. */ 
	if ((~regB & 0x2) && hour_h8) {
		systime.hour = (systime.hour + 12) % 24;
	}

	/* 2-digit to full 4-digit. */
	systime.year += 2000; // I don't think my OS can live as far to 21xx.

	if (in_ptr != NULL) {
		in_ptr->second = systime.second;
		in_ptr->minute = systime.minute;
		in_ptr->hour = systime.hour;
		in_ptr->day = systime.day;
		in_ptr->month = systime.month;
		in_ptr->year = systime.year;

		return in_ptr;
	}
	
	return &systime;
}

} /* systime */
