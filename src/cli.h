// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_SRC_CLI_H
#define __RANCH_SRC_CLI_H

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Process Ranch terminal input.
void process_input(wchar_t *cmd);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_SRC_CLI_H
