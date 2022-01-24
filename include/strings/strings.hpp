// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_STRINGS
#define __RANCH_STRINGS 1

#include <iostream>
#include <codecvt>
#include <locale>
#include <algorithm>

namespace Ranch::strings {
std::wstring string_to_wide(std::string str) noexcept;
std::wstring wleft_trim(std::wstring str) noexcept;
std::wstring wright_trim(std::wstring str) noexcept;
std::wstring wtrim(std::wstring str) noexcept;
std::string left_trim(std::string str) noexcept;
std::string right_trim(std::string str) noexcept;
std::string trim(std::string str) noexcept;
}

#endif
