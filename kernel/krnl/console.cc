
#include <stdint.h>
#include <string.h>
#include <system.h>
#include <port.h>
#include <console.h>

namespace console {

/*
 *  ------------------> y (80)
 *  | > _             |
 *  |     console     |
 *  |                 |
 *  x-----------------x
 * (25)
 */

namespace {

int attrib;

namespace cursor {

int X, Y;
bool show_cursor;
int vport; // base IO port for video, usually 0x3d4

void move(int x, int y) {
  int offset = x * VIDEO_MAX_COLUMN + y;

  port::outb(vport, 14);
  port::outb(vport | 1, offset >> 8);
  port::outb(vport, 15);
  port::outb(vport | 1, offset);
}

void push(void) {
  if (show_cursor) {
    move(X, Y);
  }
}

void pull(void) {
  int offset = 0;

  port::outb(vport, 14);
  offset = port::inb(vport | 1) << 8;
  port::outb(vport, 15);
  offset += port::inb(vport | 1);

  X = offset / VIDEO_MAX_COLUMN;
  Y = offset % VIDEO_MAX_COLUMN;
}

} /* cursor */

uint16_t *get_cell_ptr(int row, int col) {
  uint16_t *base = (uint16_t *) VIDEO_BASE;
  if (row < 0 || row >= VIDEO_MAX_ROW) {
    return NULL;
  }
  if (col < 0 || col > VIDEO_MAX_COLUMN) {
    return NULL;
  }
  return base + col + row * VIDEO_MAX_COLUMN;
}

void scroll(void) {
  uint16_t tofill = 0x20 | (attrib << 8);
  int offset = VIDEO_MAX_COLUMN * 2;

  uint16_t *ptr_first = (uint16_t *) VIDEO_BASE;
  uint16_t *ptr_second = (uint16_t *) (VIDEO_BASE + offset);

  for (int i = 0; i < VIDEO_SIZE - VIDEO_MAX_COLUMN; ++i) {
    *ptr_first++ = *ptr_second++;
  }
  for (int i = 0; i < VIDEO_MAX_COLUMN; ++i) {
    *ptr_first++ = tofill;
  }
}

}

int mkcolor(int fore, int back) {
  return (back << 4) | fore;
}

void setcolor(int color, bool reset) {
  if (reset) {
    uint8_t *ptr = (uint8_t *) VIDEO_BASE;

    ptr += 1;
    for (int i = 0; i < VIDEO_SIZE; ++i) {
      *ptr = color;
      ptr += 2;
    }
  }
  attrib = color;
}

void initialize(bool show_cursor) {
  cursor::vport = *(uint16_t *) 0x463;
  cursor::show_cursor = show_cursor;
  cursor::pull();
  if (!show_cursor) {
    /* disable cursor by hiding it. */
    cursor::move(VIDEO_MAX_ROW, 0);
  }

  attrib = mkcolor(default_fore_color, default_back_color);
  setcolor(attrib, true);
  clear();
}

void clear(void) {
  uint16_t tofill = 0x20 | (attrib << 8);
  uint16_t *ptr = (uint16_t *) VIDEO_BASE;

  for (int i = 0; i < VIDEO_SIZE; ++i) {
    *ptr++ = tofill;
  }

  cursor::X = cursor::Y = 0;
  cursor::push();
  port::wait();
}

void bkcopy(const uint16_t *src) {
  memcpy((void *) VIDEO_BASE, (void *) src, VIDEO_SIZE * 2);

  cursor::X = cursor::Y = 0;
  cursor::push();
  port::wait();
}

void putch(char ch) {
  uint16_t *ptr = get_cell_ptr(cursor::X, cursor::Y);

  if (ptr == NULL) {
    return;
  }

  if (ch == '\b') { // Backspace
    if (cursor::Y != 0) {
      cursor::Y -= 1;
    }
  } else if (ch == '\t') { // Tab
    cursor::Y = (cursor::Y + 4) & ~3;
  } else if (ch == '\r') { // Carriage return
    cursor::Y = 0;
  } else if (ch == '\n') { // New Line
    cursor::Y = 0;
    cursor::X += 1;
  } else {
    *ptr = ch | (attrib << 8);
    cursor::Y += 1;
  }
  if (cursor::Y >= VIDEO_MAX_COLUMN) {
    cursor::Y = 0;
    cursor::X += 1;
  }
  while (cursor::X >= VIDEO_MAX_ROW) {
    scroll();
    cursor::X -= 1;
  }
  cursor::push();
}

} /* console */
