// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_AST
#define __RANCH_AST 1

#include "asterror.h"
#include "../list.h"
#include "../lex/token.h"

namespace Ranch::ast {
class astbuilder {
private:
  struct list *tokens;

  inline void push_error(wchar_t *msg, const struct token *token) noexcept;
public:
  struct list *errors;

  astbuilder(struct list *tokens) noexcept;
  ~astbuilder();

  // Builds binary operation AST model.
  // Models like: [[expr], [operator], [expr], [operator], [expr]]
  // If exists any error, pushed to errors.
  // Check errors after build, AST build failed if exist any error.
  struct list *build() noexcept;
}; // class astbuilder
} // namespace Ranch::ast

#endif // __RANCH_AST
