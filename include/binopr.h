// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_BINOPR_H
#define __RANCH_BINOPR_H

#include <wchar.h>

#include "eventexpr.h"
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Binary operation.
typedef struct binopr {
  struct value      *left;
  wchar_t           *opr;
  struct value      *right;
  struct exprevents *events;
} binopr;

// Create new binopr instance allocated from heap.
struct binopr *binopr_new(void);
// Free binopr instance allocated from heap.
void binopr_free(struct binopr *bop);
// Solved the binary operation. If have any fail, return NULL.
struct value *binopr_solve(struct binopr *bop);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_BINOPR_H
