// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_ASTERROR
#define __RANCH_ASTERROR 1

#include <iostream>

namespace Ranch::ast {
struct asterror {
public:
  std::wstring msg;
  uint64_t column;

  asterror(std::wstring msg, uint64_t column) noexcept;
  ~asterror();

  friend std::wostream& operator<<(std::wostream &os, const asterror& err) {
    os << L"column: " << err.column << L" " << err.msg;
    return os;
  }
}; // struct asterror
} // namespace Ranch::ast

#endif // __RANCH_ASTERROR
