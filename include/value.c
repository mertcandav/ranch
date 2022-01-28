#include <stdlib.h>
#include <stdio.h>

#include "value.h"

value *value_new(void) {
  value *val = (value*)calloc(1, sizeof(value));
  if (val == NULL) {
    printf("error: memory allocation failed!\n");
    exit(1);
  }
  return val;
}

void value_free(value *val) {
  free(val);
  val = NULL;
}

void value_print(const value *val) {
  printf("%lf\n", val->data);
}

void value_repl(value *dest, value *src) {
  *dest = *src;
  value_free(src);
}
