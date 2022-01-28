// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_BINOPR
#define __RANCH_BINOPR 1

#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Binary operation.
typedef struct binopr {
  value      *left;
  wchar_t    *opr;
  value      *right;
  ranch_bool fail;
} binopr;

// Create new binopr instance allocated from heap.
binopr *binopr_new(void);
// Free binopr instance allocated from heap.
void binopr_free(binopr *bop);
value *binopr_solve(binopr *bop);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_BINOPR
