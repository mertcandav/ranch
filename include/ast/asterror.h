// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_ASTERROR
#define __RANCH_ASTERROR 1

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct asterror {
  wchar_t            *msg;
  unsigned long long column;
} asterror;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_ASTERROR
