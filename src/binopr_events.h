// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_SRC_EVENTS
#define __RANCH_SRC_EVENTS 1

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "../include/value.h"

volatile ranch_bool bop_failed;

void events_reset();
void event_failed(void);
void event_divided_by_zero(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_SRC_EVENTS
