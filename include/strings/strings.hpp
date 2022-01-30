// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_STRINGSCXX
#define __RANCH_STRINGSCXX 1

#include <iostream>
#include <codecvt>
#include <locale>
#include <algorithm>

namespace Ranch::strings {
std::wstring string_to_wide(const std::string str) noexcept;
std::wstring wleft_trim(std::wstring wstr) noexcept;
} // namespace Ranch::strings

#endif // __RANCH_STRINGSCXX
