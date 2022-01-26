// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_ERROR
#define __RANCH_ERROR 1

#include <iostream>

namespace Ranch::ast {
struct error {
public:
  std::wstring msg;
  uint64_t column;

  error(std::wstring msg, uint64_t column) noexcept;
  ~error();

  friend std::wostream& operator<<(std::wostream &os, const error& err) {
    os << L"column: " << err.column << L" " << err.msg;
    return os;
  }
};
}

#endif
