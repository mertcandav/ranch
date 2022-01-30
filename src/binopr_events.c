// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include <stdio.h>

#include "binopr_events.h"
#include "../include/messages.h"
#include "../include/terminal/ansi.h"
#include "../include/terminal/log.h"

void bopbase_reset(void) {
  bop_base.failed = 0;
  if (!bop_base.bop) { return; }
  expr_events_free(bop_base.bop->events);
  binopr_free(bop_base.bop);
}

void bopbase_setup(void) {
  bop_base.bop = binopr_new();
  bop_base.bop->events->failed = &event_failed;
  bop_base.bop->events->divied_by_zero = &event_divided_by_zero;
  bop_base.bop->events->modulo_by_zero = &event_modulo_by_zero;
}

void event_failed(void) {
  bop_base.failed = 1;
}

void event_divided_by_zero(void) {
  LOG_WARNING(WARNING_DIVIDED_BY_ZERO);
}

void event_modulo_by_zero(void) {
  LOG_WARNING(WARNING_MODULO_BY_ZERO);
}
