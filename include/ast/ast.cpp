#include "ast.hpp"
#include "../errors.h"
#include "../lex/id.h"
#include "../lex/tokens.h"
#include "../strings/strings.hpp"

Ranch::ast::astbuilder::astbuilder(
  std::vector<Ranch::lex::token> tokens) noexcept {
  this->tokens = tokens;
}

Ranch::ast::astbuilder::~astbuilder() {
  this->tokens.clear();
}

std::vector<std::vector<Ranch::lex::token>> Ranch::ast::astbuilder::build() noexcept {
  std::vector<std::vector<Ranch::lex::token>> processes;
  std::vector<Ranch::lex::token> part;
  bool _operator = false;
  uint64_t brace_count = 0;
  std::vector<Ranch::lex::token>::iterator it = this->tokens.begin();
  std::vector<Ranch::lex::token>::iterator end = this->tokens.end();
  for (; it < end; ++it) {
    switch ((*it).id) {
    case ID_OPERATOR:
      if (!_operator && brace_count == 0) {
        this->push_error(ERR_OPERATOR_OVERFLOW, *it);
      }
      _operator = false;
      if (brace_count > 0) {
        part.push_back(*it);
        continue;
      }
      processes.push_back(part);
      std::vector<Ranch::lex::token> operator_vector;
      operator_vector.push_back(*it);
      processes.push_back(operator_vector);
      part = std::vector<Ranch::lex::token>();
      continue;
    }
    if (_operator && brace_count == 0) {
      if ((*(it-1)).id == ID_VALUE && (*it).id == ID_VALUE) {
        this->push_error(ERR_INVALID_SYNTAX, *it);
      }
    }
    part.push_back(*it);
    _operator = true;
  }
  if (!_operator) {
    this->push_error(ERR_OPERATOR_OVERFLOW, *(it-1));
    return processes;
  }
  if (part.size() > 0) { processes.push_back(part); }
  return processes;
}

inline void Ranch::ast::astbuilder::push_error(
  std::wstring msg,
  Ranch::lex::token token) noexcept {
  this->errors.push_back(Ranch::ast::error(msg, token.column));
}
