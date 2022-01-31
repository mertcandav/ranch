#include "ast.hpp"
#include "../messages.h"
#include "../lex/id.h"
#include "../lex/tokens.h"
#include "../strings/strings.hpp"

Ranch::ast::astbuilder::astbuilder(Ranch::ast::process_tokens tokens) noexcept {
  this->tokens = tokens;
}

Ranch::ast::astbuilder::~astbuilder() {
  this->tokens.clear();
}

Ranch::ast::process_model Ranch::ast::astbuilder::build() noexcept {
  Ranch::ast::process_model processes;
  Ranch::ast::process_tokens part;
  // If it's true, needs operator.
  // If it's false, needs expression.
  bool _operator = false;
  uint64_t brace_count = 0;
  Ranch::ast::process_tokens::iterator it = this->tokens.begin();
  Ranch::ast::process_tokens::iterator end = this->tokens.end();
  for (; it < end; ++it) {
    switch ((*it)->id) {
    case ID_OPERATOR:
      if (!_operator && brace_count == 0) {
        this->push_error((wchar_t*)(ERROR_OPERATOR_OVERFLOW), *it);
      }
      _operator = false;
      if (brace_count > 0) {
        part.push_back(*it);
        continue;
      }
      processes.push_back(part);
      Ranch::ast::process_tokens operator_vector;
      operator_vector.push_back(*it);
      processes.push_back(operator_vector);
      part = Ranch::ast::process_tokens();
      continue;
    }
    if (_operator && brace_count == 0) {
      if ((*(it-1))->id == ID_VALUE && (*it)->id == ID_VALUE) {
        this->push_error((wchar_t*)(ERROR_INVALID_SYNTAX), *it);
      }
    }
    part.push_back(*it);
    _operator = true;
  }
  if (!_operator) {
    this->push_error((wchar_t*)(ERROR_OPERATOR_OVERFLOW), *(it-1));
    return processes;
  }
  if (part.size() > 0) { processes.push_back(part); }
  return processes;
}

inline void Ranch::ast::astbuilder::push_error(
  wchar_t *msg,
  struct token *tok) noexcept {
  this->errors.push_back(asterror{msg, tok->column});
}
