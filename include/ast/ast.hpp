// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_AST
#define __RANCH_AST 1

#include <vector>

#include "error.hpp"
#include "../lex/token.hpp"

namespace Ranch::ast {
class astbuilder {
private:
  std::vector<Ranch::lex::token> tokens;

  inline void push_error(std::wstring msg, Ranch::lex::token token) noexcept;
public:
  std::vector<Ranch::ast::error> errors;

  astbuilder(std::vector<Ranch::lex::token> tokens) noexcept;
  ~astbuilder();

  // Builds binary operation AST model.
  // Models like: [[expr], [operator], [expr], [operator], [expr]]
  // If exists any error, pushed to errors.
  // Check errors after build, AST build failed if exist any error.
  std::vector<std::vector<Ranch::lex::token>> build() noexcept;
};
}

#endif
