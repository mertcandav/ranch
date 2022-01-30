#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "strings.h"
#include "../messages.h"

wchar_t *wcsltrim(const wchar_t *str) {
  wchar_t *out = NULL;
  size_t len = wcslen(str);
  if (len == 0) {
    out = (wchar_t*)(malloc(sizeof(wchar_t)));
    if (!out) {
      wprintf(ERROR_ALLOCATION_FAILED L"\n");
      exit(EXIT_FAILURE);
    }
    out[0] = L'\0';
    return out;
  }
  size_t index = -1;
  while (wcs_isspace((wchar_t)(str[++index])));
  len -= index;
  out = (wchar_t*)(malloc((len+1)*sizeof(wchar_t)));
  if (!out) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  wcsncpy(out, &str[index], len);
  return out;
}

wchar_t *wcsrtrim(const wchar_t *str) {
  wchar_t *out = NULL;
  const size_t len = wcslen(str);
  if (len == 0) {
    out = (wchar_t*)(malloc(sizeof(wchar_t)));
    if (!out) {
      wprintf(ERROR_ALLOCATION_FAILED L"\n");
      exit(EXIT_FAILURE);
    }
    out[0] = L'\0';
    return out;
  }
  size_t index = len;
  while (wcs_isspace((wchar_t)(str[--index])));
  out = (wchar_t*)(malloc((index+2)*sizeof(wchar_t)));
  if (!out) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  wcsncpy(out, str, index+1);
  return out;
}

wchar_t *wcstrim(const wchar_t *str) {
  wchar_t *cache = wcsltrim(str);
  wchar_t *out = wcsrtrim(cache);
  free(cache);
  cache = NULL;
  return out;
}

unsigned char wcs_isspace(const wchar_t ch) {
  return ch == ' '  ||
         ch == '\t' ||
         ch == '\v' ||
         ch == '\r' ||
         ch == '\f';
}

unsigned char wcs_isnumber(const wchar_t ch) {
  return ch >= '0' && ch <= '9';
}

wchar_t *wcssub(const wchar_t *str, const int start) {
  wchar_t *sub = NULL;
  const size_t len = wcslen(str)-start;
  if (len < 1) {
    sub = (wchar_t*)(malloc(sizeof(wchar_t)));
    if (!sub) {
      wprintf(ERROR_ALLOCATION_FAILED L"\n");
      exit(EXIT_FAILURE);
    }
    sub[0] = L'\0';
    return sub;
  }
  sub = (wchar_t*)(malloc((len+1)*sizeof(wchar_t)));
  if (!sub) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  wcsncpy(sub, &str[start], len);
  sub[len+1] = L'\0';
  return sub;
}

wchar_t *wcsnsub(const wchar_t *str, const int start, const int n) {
  wchar_t *sub;
  if (n < 1) { goto empty; }
  const size_t len = wcslen(str)-start;
  if (len-n < 0) { goto empty; }
  sub = (wchar_t*)(malloc((n+1)*sizeof(wchar_t)));
  if (!sub) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  wcsncpy(sub, &str[start], n);
  sub[n+1] = L'\0';
  return sub;
empty:
  sub = (wchar_t*)(malloc(sizeof(wchar_t)));
  if (!sub) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  sub[0] = L'\0';
  return sub;
}

long long wcsfind(const wchar_t *str, const wchar_t *substr) {
  const wchar_t *sub = wcsstr(str, substr);
  const long long index = sub - str;
  return index < 0 ? -1 : index;
}

void wcslower(wchar_t *str) {
  size_t len = wcslen(str);
  for (size_t index = 0; index < len; ++index) {
    str[index] = towlower(str[index]);
  }
}
