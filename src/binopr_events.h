// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_SRC_EVENTS_H
#define __RANCH_SRC_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "../include/binopr.h"

// Binary operation event processor end logger.
struct binopr_base {
  volatile unsigned char failed;
} bopbase;

// Resets everything (frees binopr and events).
void bopbase_reset(void);
// Returns new setted events binopr instance allocated from heap.
struct binopr *bopbase_newbop(void);
void event_failed(void);
void event_divided_by_zero(void);
void event_modulo_by_zero(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_SRC_EVENTS_H
