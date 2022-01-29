#include <stdlib.h>
#include <stdio.h>

#include "eventexpr.h"

struct expr_events *eventexpr_new(void) {
  struct expr_events *exev = (struct expr_events*)calloc(1, sizeof(struct expr_events));
  if (exev == NULL) {
    printf("error: memory allocation failed!\n");
    exit(1);
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
