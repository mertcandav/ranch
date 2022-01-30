#include <stdlib.h>
#include <stdio.h>

#include "eventexpr.h"
#include "messages.h"

struct expr_events *eventexpr_new(void) {
  struct expr_events *exev = (struct expr_events*)calloc(1, sizeof(struct expr_events));
  if (!exev) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  exev->divied_by_zero = NULL;
  exev->failed = NULL;
  return exev;
}

void expr_events_free(struct expr_events *exev) {
  free(exev);
  exev = NULL;
}

void expr_events_invoke(const void(*e)(void)) {
  if (e == NULL) { return; }
  (*e)();
}
