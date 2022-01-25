// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_LEXER
#define __RANCH_LEXER 1

#include <iostream>
#include <vector>

#include "token.hpp"
#include "../terminal/ansi/ansi.h"

namespace Ranch::lex {
class lexer {
private:
  bool finished;
  std::wstring text;
  uint64_t column;

  void resume(void) noexcept;
  bool lex_operator(std::wstring text, Ranch::lex::token *token) noexcept;
  void error(void) noexcept;
public:
  lexer(std::wstring text) noexcept;
  ~lexer();

  inline void reset(void) noexcept;
  std::vector<Ranch::lex::token> lex(void) noexcept;
  Ranch::lex::token next(void) noexcept;
  constexpr inline bool ended(void) const noexcept;
};
}

#endif
