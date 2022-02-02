// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

// This header includes message defines.

#ifndef __RANCH_ERRORS_H
#define __RANCH_ERRORS_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define ERROR_ALLOCATION_FAILED L"ERROR: Memory allocation is failed!"
#define ERROR_NOTEXIST_COMMAND L"ERROR: There is not such command!"
#define ERROR_COMMAND_NOTALONE L"ERROR: This command is should be single!"
#define ERROR_OPERATOR_OVERFLOW L"ERROR: Operator overlow!"
#define ERROR_INVALID_SYNTAX L"ERROR: Invalid syntax!"
#define ERROR_COMPUTED_FAILED L"ERROR: Computed failed!"
#define ERROR_FLOAT_POINT_OVERFLOW L"ERROR: Double pointed float literal!"

#define WARNING_DIVIDED_BY_ZERO L"WARN: Divided by zero, result computed as zero!"
#define WARNING_MODULO_BY_ZERO L"WARN: Modulo by zero, result computed as zero!"

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_ERRORS_H
