/* warning: this code is not carefully tested. */

#include <stdint.h>
#include <stdio.h>
#include <abi.h>
#include <panic.h>
#include <pair>
#include <vector>
#include <algorithm>

using std::vector;
using std::pair;
using std::make_pair;
using std::sort;

typedef void (* fn_ptr) (void *);
typedef pair<void *, int> atexit_info_t; // pobj, insert_index

namespace abi {

namespace {

struct atexit_t {
  fn_ptr lpfn;
  vector<atexit_info_t> atexits;
  atexit_t *next;
};

#define SLOT_SIZE 131  // prime

atexit_t *slot_header[SLOT_SIZE];
size_t cur_insert_index;

uint32_t hash(fn_ptr lpfn) {
  return uint32_t(lpfn) * uint32_t(2654435761);
}

/* we don't need dso_handle actually. */
int cxa_atexit(fn_ptr lpfn, void *pobj, void * /* dso_handle */) {
  printf("[cxa_atexit] Register lpfn = 0x%x, pobj = 0x%x\n", lpfn, pobj);

  uint32_t h = hash(lpfn) % SLOT_SIZE;
  atexit_t *patexit = slot_header[h];

  while (patexit != NULL) {
    if (patexit->lpfn == lpfn) {
      break;
    } else {
      patexit = patexit->next;
    }
  }

  if (patexit == NULL) {
    patexit = new atexit_t();
    patexit->lpfn = lpfn;
    patexit->next = slot_header[h];
    slot_header[h] = patexit;
  }

  patexit->atexits.push_back(make_pair(pobj, ++cur_insert_index));

  return 0;
}

void cxa_finalize(fn_ptr lpfn) {
  if (lpfn != NULL) {
    uint32_t h = hash(lpfn) % SLOT_SIZE;
    atexit_t *patexit = slot_header[h], *prev = NULL;

    while (patexit != NULL) {
      if (patexit->lpfn == lpfn) {
        break;
      } else {
        prev = patexit;
        patexit = patexit->next;
      }
    }
    if (patexit == NULL) {
      /* key lpfn is not found in the table .*/
      panic::panic("[cxa_finalize] Bad function pointer.");
    }
    for (int i = int(patexit->atexits.size()) - 1; i >= 0; --i) {
      patexit->lpfn(patexit->atexits[i].first);
    }
    if (prev == NULL) {
      slot_header[h] = patexit->next;
    } else {
      prev->next = patexit->next;
    }
    delete patexit;
  } else {
    vector<pair<fn_ptr, atexit_info_t>> all;
    for (int i = 0; i < SLOT_SIZE; ++i) {
      atexit_t *p = slot_header[i];
      while (p != NULL) {
        for (auto &item : p->atexits) {
          all.push_back(make_pair(p->lpfn, item));
        }
        auto backup = p;
        p = p->next;
        delete backup;
      }
    }
    sort(all.begin(), all.end(), [&](const pair<fn_ptr, atexit_info_t> &a, const pair<fn_ptr, atexit_info_t> &b) -> bool {
      return a.second.second > b.second.second; // compared by insert order
    });
    for (auto it = all.begin(); it != all.end(); ++it) {
      it->first(it->second.first);
    }
  }
}

}

} /* abi */

/* exports go here. */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void *__dso_handle = 0;

int __cxa_atexit(fn_ptr lpfn, void *pobj, void *dso_handle) {
  return abi::cxa_atexit(lpfn, pobj, dso_handle);
}

void __cxa_finalize(fn_ptr lpfn) {
  abi::cxa_finalize(lpfn);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

namespace abi {

void dtors(void) {
  __cxa_finalize(NULL);
}

} /* abi */
