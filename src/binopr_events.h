// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_SRC_EVENTS
#define __RANCH_SRC_EVENTS 1

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "../include/binopr.h"

// Binary operation event processor end logger.
struct binopr_base {
  volatile unsigned char failed;
  struct binopr         *bop;
} bop_base;

// Resets everything (frees binopr and events).
void bopbase_reset(void);
// Setup bopbase (not resets old, allocates binopr from heap).
void bopbase_setup(void);
void event_failed(void);
void event_divided_by_zero(void);
void event_modulo_by_zero(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_SRC_EVENTS
