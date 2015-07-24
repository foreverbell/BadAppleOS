#include <system.h>
#include <memory.h>
#include <console.h>
#include <stdio.h>
#include <stdint.h>
#include <systime.h>
#include <vector>
#include <pair>
#include "video.h"

using systime::systime_t;
using systime::get_systime;

namespace {

badapple::video v;

void end_callback(uint64_t, int) {
	systime_t *st = get_systime(NULL);
	for (int i = 0; i < 41; ++i) { // 41 is length of next display text
		console::putch('\b');
	}
	printf("Current system time = %d/%02d/%02d %02d:%02d:%02d", st->year, st->month, st->day, st->hour, st->minute, st->second);	
}

void video_callback(uint64_t tick, int id) {
	if (tick & 1) { // fps = 9
		if (v.has_next()) {
			v.next();
			printf(" (%d%%) ", v.progress());
		} else {
			v.free();
			timer::remove(id);
			console::clear();
			printf("Thank you for watching!\n");
			printf("Repository can be found at https://github.com/foreverbell/BadAppleOS.\n");
			timer::add(end_callback);
		}
	}
}

}

void play(void) {
	timer::add(video_callback);
}
