// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_STRINGS
#define __RANCH_STRINGS 1

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

wchar_t *wcsltrim(const wchar_t *str);
wchar_t *wcsrtrim(const wchar_t *str);
wchar_t *wcstrim(const wchar_t *str);
wchar_t *wcssub(const wchar_t *str, int start);
wchar_t *wcsnsub(const wchar_t *str, int start, int n);
unsigned char wcs_isspace(const wchar_t ch);
unsigned char wcs_isnumber(const wchar_t ch);
long long wcsfind(const wchar_t *str, const wchar_t *substr);
void wcslower(wchar_t *str);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_STRINGS
