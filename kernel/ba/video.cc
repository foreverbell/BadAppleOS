
#include <system.h>
#include <console.h>
#include <stdio.h>
#include <stdint.h>
#include <new>
#include <vector>
#include <pair>
#include "badapple.h"

using std::vector;
using std::pair;
using std::make_pair;

namespace badapple {

extern uint8_t vdatas[] asm("_binary_vdata_bin_start");
extern uint8_t vdatae[] asm("_binary_vdata_bin_end");

int video::progress() const {
  return (cur_frame + 1) * 100 / count;
}

bool video::has_next() const {
  return cur_frame < count;
}

void video::next() {
  console::bkcopy(pool + cur_frame * VIDEO_SIZE);
  cur_frame += 1;
}

void video::free(void) {
  delete [] pool;
}

video::video() {
#ifdef VIDEO_SQUARE_TEXT
  const uint16_t bkcolor1 = console::mkcolor(console::vga_color::white, console::vga_color::black);
  const uint16_t bkcolor2 = console::mkcolor(console::vga_color::black, console::vga_color::white);
#else
  const uint16_t acolor = console::mkcolor(console::vga_color::light_grey, console::vga_color::black);
#endif

  srand(1024);

  decompressor decomp(vdatas, vdatae);

  cur_frame = 0;
  count = decomp.frame_count();
  pool = new uint16_t[count * VIDEO_SIZE];

  printf("[video] Frame count = %d.\n", count);
  printf("[video] Pool address = 0x%x.\n", (int) pool);

  stream reader = stream(decomp.data(), decomp.data_end());
  uint16_t *pool_ptr = pool;
  uint16_t attrib;

  while (reader.has_next()) {
    int bit = reader.nextb();
#ifdef VIDEO_SQUARE_TEXT
    attrib = (bit ? bkcolor1 : bkcolor2) << 8;
#else
    attrib = (acolor << 8) | (bit ? ' ' : '#');
#endif
    *pool_ptr++ = attrib;
  }
  printf("[video] Data loaded, tailing pointer = 0x%x.\n", (int) pool_ptr);

#ifndef VIDEO_SQUARE_TEXT
  puts("[video] Artifying data, this may take a few second.");
  artify();
  puts("[video] Data artified.");
#endif
}

std::pair<int, video::info_t> video::find(int x) {
  if (dsu[x].first != x) {
    dsu[x] = find(dsu[x].first);
  }
  return dsu[x];
}

void video::join(int x, int y) {
  x = find(x).first;
  y = find(y).first;
  if (x != y) {
    if (dsu[x].second.ch != '?') {
      std::swap(x, y);
    }
    dsu[x].first = dsu[y].first;
    dsu[y].second.size += dsu[x].second.size;
  }
}

void video::artify() {
  const int dxy[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  const char dot_chars[] = {'\'', '`', ',', '.'};
  const char line_chars[] = {'\\', '/', char(28), '^'};
  const char dense_chars[] = {'?', 'o', '%'};
  const int dense_count = sizeof(dense_chars) / sizeof(char);

  dsu.resize(VIDEO_SIZE);

  auto at = [&](int frame, int x, int y) -> char {
    const uint16_t *p = pool;
    p += frame * VIDEO_SIZE;
    p += y * VIDEO_MAX_COLUMN;
    p += x;
    return *(const char *) p;
  };
  auto set = [&](int frame, int x, int y, char ch) -> void {
    const uint16_t *p = pool;
    p += frame * VIDEO_SIZE;
    p += y * VIDEO_MAX_COLUMN;
    p += x;
    *(char *) p = ch;
  };
  auto label = [&](int x, int y) -> int {
    return y * VIDEO_MAX_COLUMN + x;
  };
  auto within = [&](int x, int y) -> bool {
    return x >= 0 && x < VIDEO_MAX_COLUMN && y >= 0 && y < VIDEO_MAX_ROW;
  };

  /* randomly change connected block to one dense char. */
  for (int f = 0; f < count; ++f) {
    for (int x = 0; x < VIDEO_MAX_COLUMN; ++x) {
      for (int y = 0; y < VIDEO_MAX_ROW; ++y) {
        char ch = at(f, x, y);
        if (ch != ' ') {
          if (f > 0 && at(f - 1, x, y) != ' ') {
            ch = at(f - 1, x, y);
          } else {
            ch = '?'; // to be determinated.
          }
        }
        dsu[label(x, y)] = make_pair(label(x, y), (info_t) {ch, 1} );
      }
    }
    for (int x = 0; x < VIDEO_MAX_COLUMN; ++x) {
      for (int y = 0; y < VIDEO_MAX_ROW; ++y) {
        if (at(f, x, y) == ' ') {
          continue;
        }
        for (int d = 0; d < 4; ++d) {
          if (!within(x + dxy[d][0], y + dxy[d][1]) || at(f, x + dxy[d][0], y + dxy[d][1]) == ' ') {
            continue;
          }
          join(label(x, y), label(x + dxy[d][0], y + dxy[d][1]));
        }
      }
    }
    for (int x = 0; x < VIDEO_MAX_COLUMN; ++x) {
      for (int y = 0; y < VIDEO_MAX_ROW; ++y) {
        int father = find(label(x, y)).first;
        char &ch = dsu[father].second.ch;
        /* MSB=1 <---> mutated. */
        if (ch == '?') {
          ch = dense_chars[rand() % dense_count] | 0x80;
        } else if (ch > 0 && ch != ' ' && dsu[father].second.size < 100) {
          /* 10% possibility to get a mutation. */
          ch = rand() % 10 == 0 ? dense_chars[rand() % dense_count] : ch;
          ch |= 0x80;
        }
        set(f, x, y, ch & 0x7f);
      }
    }
  }

  /* emphasize the boundary. */
  for (int f = 0; f < count; ++f) {
    for (int x = 0; x < VIDEO_MAX_COLUMN; ++x) {
      for (int y = 0; y < VIDEO_MAX_ROW; ++y) {
        if (at(f, x, y) == ' ') {
          continue;
        }
        int dir = -1, empty = 0;
        for (int d = 0; d < 4; ++d) {
          if (!within(x + dxy[d][0], y + dxy[d][1])) {
            continue;
          }
          if (at(f, x + dxy[d][0], y + dxy[d][1]) == ' ') {
            dir = d;
            empty += 1;
          }
        }
        if (dir != -1) {
          bool use_line = true;
          if (empty > 2) {
            use_line = false;
          }
          if (within(x + dxy[(3 - dir) ^ 1][0], y + dxy[(3 - dir) ^ 1][1]) &&
              at(f, x + dxy[(3 - dir) ^ 1][0], y + dxy[(3 - dir) ^ 1][1]) == ' ') {
            use_line = false;
          }
          if (within(x + dxy[3 - dir][0], y + dxy[3 - dir][1]) &&
              at(f, x + dxy[3 - dir][0], y + dxy[3 - dir][1]) == ' ') {
            use_line = false;
          }
          set(f, x, y, use_line ? line_chars[dir] : dot_chars[dir]);
        }
      }
    }
  }
}

} /* badapple */
