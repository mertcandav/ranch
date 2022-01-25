// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_TOKEN
#define __RANCH_TOKEN 1

#include <iostream>
#include "id.h"

namespace Ranch::lex {
struct token {
public:
  uint16_t id;
  std::wstring kind;
  uint64_t column;

  token() noexcept;
  token(uint16_t id, std::wstring kind, uint64_t column) noexcept;
  ~token();
};
}

#endif
