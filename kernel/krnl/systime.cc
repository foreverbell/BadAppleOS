
#include <system.h>
#include <port.h>
#include <systime.h>

namespace systime {

namespace {

const int cmos_addr = 0x70;
const int cmos_data = 0x71;

systime_t systime;

int read_cmos(int reg) {
#ifdef NMI_DISABLE
	reg |= (1 << 7);
#endif
	outportb(cmos_addr, reg);
	return inportb(cmos_data);
}

bool update_in_progress() {
	int flag = read_cmos(0xa) & 0x80;
	return (flag ? true : false);
}

void fill_systime(systime_t *in_ptr) {
	while (update_in_progress());
	in_ptr->second = read_cmos(0x0);
	in_ptr->minute = read_cmos(0x2);
	in_ptr->hour = read_cmos(0x4);
	in_ptr->day = read_cmos(0x7);
	in_ptr->month = read_cmos(0x8);
	in_ptr->year = read_cmos(0x9);
}

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
#define BCD_TO_BINARY(x) (((x) & 0xf) + (((x) >> 4) * 10))
		systime.second = BCD_TO_BINARY(systime.second);
		systime.minute = BCD_TO_BINARY(systime.minute);
		systime.hour = BCD_TO_BINARY(systime.hour);
		systime.day = BCD_TO_BINARY(systime.day);
		systime.month = BCD_TO_BINARY(systime.month);
		systime.year = BCD_TO_BINARY(systime.year);
#undef BCD_TO_BINARY
	}

	/* convert 12 hour to 24 hour if necessary. */ 
	if ((~regB & 0x2) && hour_h8) {
		systime.hour = (systime.hour + 12) % 24;
	}

	/* 2-digit to full 4-digit. */
	systime.year += 2000; // I don't think my OS can live that far to 21xx.

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
