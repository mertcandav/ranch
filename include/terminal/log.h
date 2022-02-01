// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_LOG_H
#define __RANCH_LOG_H

#include <stdio.h>

#include "ansi.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define LOG_ERROR(msg) wprintf(COLOR_RED msg FONT_RESET L"\n")
#define LOG_WARNING(msg) wprintf(COLOR_YELLOW msg FONT_RESET L"\n")

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_LOG_H
