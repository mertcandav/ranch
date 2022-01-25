// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include "strings.hpp"

std::wstring Ranch::strings::string_to_wide(const std::string str) noexcept {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  return converter.from_bytes(str);
}

std::wstring Ranch::strings::wleft_trim(std::wstring wstr) noexcept {
  wstr.erase(wstr.begin(), std::find_if(wstr.begin(), wstr.end(), [](wchar_t wch) {
    return !std::isspace(wch);
  }));
  return wstr;
}

std::wstring Ranch::strings::wright_trim(std::wstring wstr) noexcept {
  wstr.erase(std::find_if(wstr.rbegin(), wstr.rend(), [](wchar_t wch) {
    return !std::isspace(wch);
  }).base(), wstr.end());
  return wstr;
}

std::wstring Ranch::strings::wtrim(std::wstring wstr) noexcept {
  return wleft_trim(wright_trim(wstr));
}

bool Ranch::strings::is_space(const wchar_t wch) noexcept {
  return wch == L' '  ||
         wch == L'\t' ||
         wch == L'\v' ||
         wch == L'\r' ||
         wch == L'\f';
}

bool Ranch::strings::is_number(const wchar_t wch) noexcept {
  return wch >= '0' && wch <= '9';
}
