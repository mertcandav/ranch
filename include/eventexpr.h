// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_EVENTEXPR_H
#define __RANCH_EVENTEXPR_H

#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct expr_events {
  void(*divied_by_zero)(void);
  void(*modulo_by_zero)(void);
  void(*failed)(void);
} expr_events;

// Create new eventexpr instance allocated from heap.
struct expr_events *eventexpr_new(void);
// Free eventexpr instance allocated from heap.
void expr_events_free(struct expr_events *exev);
// Invokes event.
void expr_events_invoke(void(*e)(void));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_EVENTEXPR_H
