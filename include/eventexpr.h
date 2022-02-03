// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_EVENTEXPR_H
#define __RANCH_EVENTEXPR_H

#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct exprevents {
  void(*divied_by_zero)(void);
  void(*modulo_by_zero)(void);
  void(*failed)(void);
} exprevents;

// Create new exprevents instance allocated from heap.
struct exprevents *eventexpr_new(void);
// Free exprevents instance allocated from heap.
void expr_events_free(struct exprevents *exev);
// Invokes event.
void expr_events_invoke(void(*e)(void));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_EVENTEXPR_H
