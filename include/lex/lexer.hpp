// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_LEXER
#define __RANCH_LEXER 1

#include <iostream>
#include <vector>

#include "token.h"

namespace Ranch::lex {
class lexer {
private:
  bool failed;
  bool finished;
  // Text to lex.
  std::wstring text;
  // Current lexer column.
  // This is a index value actually.
  uint64_t column;

  // Resume from last column.
  // Skips if starts with spaces or something ignored values.
  void resume(void) noexcept;
  bool lex_operator(const std::wstring text, struct token *tok) noexcept;
  bool lex_numeric(std::wstring text, struct token *tok) noexcept;
  // Prints error and stops lexing and frees generated tokens.
  void error(void) noexcept;
public:
  lexer(std::wstring text) noexcept;
  ~lexer();

  // Resets everything, returns to start.
  inline void reset(void) noexcept;
  // Lexs and returns all tokens of text.
  // If exist any tokenization error, prints and stops.
  std::vector<struct token*> lex(void) noexcept;
  // Lexs and returns next token.
  struct token *next(void) noexcept;
  bool ended(void) const noexcept;
  // Returns lexing failed or not.
  bool fail(void) const noexcept;
}; // class lexer
} // namespace Ranch::lex

#endif // __RANCH_LEXER
