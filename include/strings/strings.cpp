// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include "strings.hpp"

std::wstring Ranch::strings::string_to_wide(const std::string str) noexcept {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  return converter.from_bytes(str);
}

std::wstring Ranch::strings::wleft_trim(std::wstring str) noexcept {
  str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
    return !std::isspace(ch);
  }));
  return str;
}

std::wstring Ranch::strings::wright_trim(std::wstring str) noexcept {
  str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
    return !std::isspace(ch);
  }).base(), str.end());
  return str;
}

std::wstring Ranch::strings::wtrim(std::wstring str) noexcept {
  return wleft_trim(wright_trim(str));
}

std::string Ranch::strings::left_trim(std::string str) noexcept {
  str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
    return !std::isspace(ch);
  }));
  return str;
}

std::string Ranch::strings::right_trim(std::string str) noexcept {
  str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
    return !std::isspace(ch);
  }).base(), str.end());
  return str;
}

inline std::string Ranch::strings::trim(std::string str) noexcept {
  return left_trim(right_trim(str));
}

inline bool Ranch::strings::is_space(const char ch) noexcept {
  return ch == ' '  ||
         ch == '\t' ||
         ch == '\v' ||
         ch == '\r' ||
         ch == '\f';
}

bool Ranch::strings::wis_space(const wchar_t wch) noexcept {
  return wch == L' '  ||
         wch == L'\t' ||
         wch == L'\v' ||
         wch == L'\r' ||
         wch == L'\f';
}
