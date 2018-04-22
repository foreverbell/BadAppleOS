
#include <stdio.h>
#include <string.h>
#include <new>
#include <memory.h>
#include "badapple.h"

namespace badapple {

namespace btrie {

struct node_t {
  node_t *child[2];
  int label;
} root;

void insert(node_t *p, const int8_t *pdat, int label) {
  while (*pdat != -1) {
    int data = *pdat;
    if (p->child[data] == NULL) {
      p->child[data] = new node_t();
      memset(p->child[data], 0, sizeof(node_t));
      p->child[data]->label = -1;
    }
    p = p->child[data];
    pdat += 1;
  }
  p->label = label;
}

void free(node_t *p) {
  for (int i = 0; i < 2; ++i) {
    if (p->child[i] != NULL) {
      free(p->child[i]);
    }
  }
  if (p != &root) {
    delete [] p;
  }
}

} /* btrie */

decompressor::decompressor(const uint8_t *vdatas, const uint8_t *vdatae) {
  auto read = [&](const uint8_t *data, int bytes) -> int {
    int ret = 0;
    while (bytes-- > 0) {
      ret = (ret << 8) | *data++;
    }
    return ret;
  };
  int buffer_size, key_count;

  count = read(vdatas, 2); // frame_count
  buffer_size = read(vdatas + 2, 4);
  key_count = read(vdatas + 6, 1);

  printf("[decompressor] Frame count = %d.\n", count);
  printf("[decompressor] Buffer size = %d.\n", buffer_size);
  printf("[decompressor] Key count = %d.\n", key_count);

  buffer = new uint8_t[buffer_size];
  buffer_end = buffer + buffer_size;

  const uint8_t *pointer = vdatas + 7;

  mm::log_status(MM_LOG_SILENT); { // tell memory logger to shut up

    for (int i = 0; i < key_count; ++i) {
      int key = (int) *pointer++;
      int length = (int) *pointer++;
      static int8_t data[256];
      data[length] = -1;
      stream reader = stream(pointer, pointer + (length - 1) / 8 + 1);
      for (int i = 0; i < length; ++i) {
        data[i] = reader.nextb();
      }
      btrie::insert(&btrie::root, data, key);
      pointer += (length - 1) / 8 + 1;
    }

    btrie::node_t *cur = &btrie::root;
    stream reader = stream(pointer, vdatae);
    for (int i = 0; i < buffer_size; ++i) {
      while (true) {
        int bit = reader.nextb();
        cur = cur->child[bit];
        if (cur->label != -1) {
          buffer[i] = cur->label;
          cur = &btrie::root;
          break;
        }
      }
    }

    btrie::free(&btrie::root);

    printf("[decompressor] Remaining %d bits.\n", reader.remain());

  } mm::log_status(MM_LOG_NOISY);
}

decompressor::~decompressor() {
  delete [] buffer;
}

} /* badapple */
