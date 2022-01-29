// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include <stdio.h>

#include "binopr_events.h"
#include "../include/messages.h"
#include "../include/terminal/ansi.h"
#include "../include/terminal/log.h"

void events_reset(void) {
  event_logs.bop_failed = 0;
}

void event_failed(void) {
  event_logs.bop_failed = 1;
}

void event_divided_by_zero(void) {
  LOG_WARNING(WARNING_DIVIDED_BY_ZERO);
}
