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
std::wstring wtrim(const std::wstring wstr) noexcept;
std::wstring to_lower(std::wstring wstr) noexcept;
bool is_space(const wchar_t wch) noexcept;
bool is_number(const wchar_t wch) noexcept;
}

#endif
