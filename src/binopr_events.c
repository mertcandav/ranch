// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include <stdio.h>

#include "binopr_events.h"
#include "../include/messages.h"
#include "../include/terminal/ansi.h"
#include "../include/terminal/log.h"

void bopbase_reset(void)
{ bopbase.failed = 0; }

struct binopr *bopbase_newbop(void) {
  struct binopr *bop = binopr_new();
  bop->events->failed = &event_failed;
  bop->events->divied_by_zero = &event_divided_by_zero;
  bop->events->modulo_by_zero = &event_modulo_by_zero;
  return bop;
}

void event_failed(void)
{ bopbase.failed = 1; }

void event_divided_by_zero(void)
{ LOG_WARNING(WARNING_DIVIDED_BY_ZERO); }

void event_modulo_by_zero(void)
{ LOG_WARNING(WARNING_MODULO_BY_ZERO); }
