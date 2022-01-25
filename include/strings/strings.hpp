// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_STRINGS
#define __RANCH_STRINGS 1

#include <iostream>
#include <codecvt>
#include <locale>
#include <algorithm>

namespace Ranch::strings {
std::wstring string_to_wide(const std::string str) noexcept;
std::wstring wleft_trim(std::wstring wstr) noexcept;
std::wstring wright_trim(std::wstring wstr) noexcept;
std::wstring wtrim(std::wstring wstr) noexcept;
std::string left_trim(std::string str) noexcept;
std::string right_trim(std::string str) noexcept;
inline std::string trim(std::string str) noexcept;
inline bool is_space(const char ch) noexcept;
bool is_space(const wchar_t wch) noexcept;
inline bool is_number(const char ch) noexcept;
bool is_number(const wchar_t wch) noexcept;
}

#endif
