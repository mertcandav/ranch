// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_AST
#define __RANCH_AST 1

#include <vector>

#include "asterror.h"
#include "../lex/token.hpp"

namespace Ranch::ast {
typedef std::vector<Ranch::lex::token> process_tokens;
typedef std::vector<Ranch::ast::process_tokens> process_model;

class astbuilder {
private:
  Ranch::ast::process_tokens tokens;

  inline void push_error(wchar_t *msg, Ranch::lex::token token) noexcept;
public:
  std::vector<struct asterror> errors;

  astbuilder(Ranch::ast::process_tokens tokens) noexcept;
  ~astbuilder();

  // Builds binary operation AST model.
  // Models like: [[expr], [operator], [expr], [operator], [expr]]
  // If exists any error, pushed to errors.
  // Check errors after build, AST build failed if exist any error.
  Ranch::ast::process_model build() noexcept;
}; // class astbuilder
} // namespace Ranch::ast

#endif // __RANCH_AST
