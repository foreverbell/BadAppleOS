#include <timer.h>
#include <memory.h>
#include <console.h>
#include <stdio.h>
#include <stdint.h>
#include <systime.h>
#include <vector>
#include <pair>
#include "badapple.h"

using systime::systime_t;
using systime::get_systime;
using timer::handle_t;

namespace {

badapple::video v;

}

void play(void) {
	const int fps = 9;
	
	timer::add(TIMER_TICK_PER_SECOND, [&](uint64_t count, handle_t h) -> void {
		if (count < 3) {
			printf("\rBadApple, %d second(s) to go", 3 - count);
			for (uint64_t i = 0; i <= count; ++i) {
				printf(".");
			}
		} else {
			timer::remove(h);
			timer::add(TIMER_TICK_PER_SECOND / fps, [&](uint64_t, handle_t h) -> void {
				if (v.has_next()) {
					v.next();
					printf(" (%d%%) ", v.progress());
				} else {
					mm::log_status(MM_LOG_SILENT);
						
					timer::remove(h);
					v.free();
					console::clear();
					printf("Thank you for watching!\n");
					printf("Repository can be found at https://github.com/foreverbell/BadAppleOS.\n");
					timer::add(1, [&](uint64_t, handle_t) -> void {
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
