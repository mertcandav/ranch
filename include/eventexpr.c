#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include "eventexpr.h"
#include "messages.h"

struct exprevents *eventexpr_new(void) {
  struct exprevents *exev = (struct exprevents*)malloc(sizeof(struct exprevents));
  if (!exev) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  exev->modulo_by_zero = NULL;
  exev->divied_by_zero = NULL;
  exev->failed = NULL;
  return exev;
}

void expr_events_free(struct exprevents *exev) {
  free(exev);
  exev = NULL;
}

void expr_events_invoke(void(*e)(void)) {
  if (!e) { return; }
  (*e)();
}
