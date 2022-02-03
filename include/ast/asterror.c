#include <stdlib.h>
#include <stdio.h>

#include "asterror.h"
#include "../messages.h"

struct asterror *asterror_new(void) {
  struct asterror *err = (struct asterror*)(malloc(sizeof(struct asterror)));
  if (!err) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  err->column = 0;
  err->message = NULL;
  return err;
}

void asterror_free(struct asterror *err) {
  free(err);
  err = NULL;
}

void asterror_print(const struct asterror *err)
{ wprintf(L"Column: %d %ls\n", err->column, err->message); }
