// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_ASTERROR_H
#define __RANCH_ASTERROR_H

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct asterror {
  wchar_t            *message;
  unsigned long long column;
} asterror;

// Create new asterror instance allocated from heap.
struct asterror *asterror_new(void);
// Free asterror instance allocated from heap.
void asterror_free(struct asterror *err);
// Prints as log the asterror.
void asterror_print(const struct asterror *err);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_ASTERROR_H
