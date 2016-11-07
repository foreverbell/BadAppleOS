
#include <system.h>
#include <port.h>
#include <systime.h>

using cmos::read;

namespace systime {

namespace {

systime_t systime;

void try_get(systime_t *in_ptr) {
  /* check if CMOS is updating data. */
  while (read(0xa) & 0x80) {
    __asm__ __volatile__ ("nop");
  }
  in_ptr->second = read(0x0);
  in_ptr->minute = read(0x2);
  in_ptr->hour   = read(0x4);
  in_ptr->day    = read(0x7);
  in_ptr->month  = read(0x8);
  in_ptr->year   = read(0x9);
}

}

systime_t *get_systime(systime_t *in_ptr) {
  systime_t cur_systime;

  for (try_get(&systime); ; ) {
    try_get(&cur_systime);
    if (cur_systime == systime) {
      break;
    }
    systime = cur_systime;
  }

  /* read status register B. */
  int status_B = read(0xb);

  /* read and clear 0x80 bit of hour. */
  int hour_h8 = systime.hour & 0x80;
  systime.hour &= ~0x80;

  if (~status_B & 0x4) {
    /* detected BCD mode, convert to binary mode. */
#define BCD_TO_BINARY(x) (((x) & 0xf) + (((x) >> 4) * 10))
    systime.second = BCD_TO_BINARY(systime.second);
    systime.minute = BCD_TO_BINARY(systime.minute);
    systime.hour   = BCD_TO_BINARY(systime.hour);
    systime.day    = BCD_TO_BINARY(systime.day);
    systime.month  = BCD_TO_BINARY(systime.month);
    systime.year   = BCD_TO_BINARY(systime.year);
#undef BCD_TO_BINARY
  }

  /* convert 12 hour to 24 hour if necessary. */
  if ((~status_B & 0x2) && hour_h8) {
    systime.hour = (systime.hour + 12) % 24;
  }

  /* 2-digit to full 4-digit. */
  systime.year += 2000; // I don't think my OS can live that far to 21xx.

  if (in_ptr != NULL) {
    in_ptr->second = systime.second;
    in_ptr->minute = systime.minute;
    in_ptr->hour   = systime.hour;
    in_ptr->day    = systime.day;
    in_ptr->month  = systime.month;
    in_ptr->year   = systime.year;

    return in_ptr;
  }

  return &systime;
}

} /* systime */
