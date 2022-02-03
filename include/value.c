#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include "value.h"
#include "messages.h"

struct value *value_new(void) {
  struct value *val = (struct value*)malloc(sizeof(struct value));
  if (!val) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  val->data = 0;
  return val;
}

void value_free(struct value *val) {
  free(val);
  val = NULL;
}

void value_print(const struct value *val)
{ printf("%lf\n", val->data); }

void value_repl(struct value *dest, struct value *src) {
  *dest = *src;
  value_free(src);
}
