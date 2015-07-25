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

}

void play(void) {
	const int fps = 9;
	
	timer::add(TIMER_TICK_ONE_SECOND, [&](uint64_t count, int id) -> void {
		if (count < 3) {
			printf("\rBadApple, %d second(s) to go", 3 - count);
			for (uint64_t i = 0; i <= count; ++i) {
				printf(".");
			}
		} else {
			timer::remove(id);
			timer::add(TIMER_TICK_ONE_SECOND / fps, [&](uint64_t, int id) -> void {
				if (v.has_next()) {
					v.next();
					printf(" (%d%%) ", v.progress());
				} else {
					timer::remove(id);
					v.free();
					console::clear();
					printf("Thank you for watching!\n");
					printf("Repository can be found at https://github.com/foreverbell/BadAppleOS.\n");
					timer::add(1, [&](uint64_t, int) -> void {
						systime_t *st = get_systime(NULL);
						printf("\rCurrent system time = %d/%02d/%02d %02d:%02d:%02d.", 
							st->year, st->month, st->day, 
							st->hour, st->minute, st->second);	
					});
				}
			});
		}
	});
}
