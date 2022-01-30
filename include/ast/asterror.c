#include <stdio.h>

#include "asterror.h"

void asterror_print(const struct asterror err) {
  wprintf(L"Column: %d %ls\n", err.column, err.msg);
}
