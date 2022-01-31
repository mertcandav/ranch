#include "ast.hpp"
#include "../messages.h"
#include "../lex/id.h"
#include "../lex/tokens.h"
#include "../strings/strings.hpp"

Ranch::ast::astbuilder::astbuilder(struct list *tokens) noexcept {
  this->errors = list_new(0);
  this->tokens = tokens;
}

Ranch::ast::astbuilder::~astbuilder() {
  list_free(this->errors);
}

struct list *Ranch::ast::astbuilder::build() noexcept {
  struct list *processes = list_new(1);
  struct list *part = list_new(1);
  // If it's true, needs operator.
  // If it's false, needs expression.
  bool _operator = false;
  uint64_t brace_count = 0;
  for (size_t index = 0; index < this->tokens->used; ++index) {
    struct token *tok = (struct token*)(this->tokens->array[index]);
    switch (tok->id) {
    case ID_OPERATOR:
      if (!_operator && brace_count == 0) {
        this->push_error((wchar_t*)(ERROR_OPERATOR_OVERFLOW), tok);
      }
      _operator = false;
      if (brace_count > 0) {
        list_push(part, tok);
        continue;
      }
      list_push(processes, part);
      struct list *operator_list = list_new(1);
      list_push(operator_list, tok);
      list_push(processes, operator_list);
      part = list_new(0);
      continue;
    }
    if (_operator && brace_count == 0) {
      const struct token *prev_tok = (struct token*)(this->tokens->array[index-1]);
      if (prev_tok->id == ID_VALUE && tok->id == ID_VALUE) {
        this->push_error((wchar_t*)(ERROR_INVALID_SYNTAX), tok);
      }
    }
    list_push(part, tok);
    _operator = true;
  }
  if (!_operator) {
    const struct token *last_tok = (struct token*)(this->tokens->array[this->tokens->used-1]);
    this->push_error((wchar_t*)(ERROR_OPERATOR_OVERFLOW), last_tok);
    return processes;
  }
  if (part->used > 0) { list_push(processes, part); }
  return processes;
}

inline void Ranch::ast::astbuilder::push_error(
  wchar_t *msg,
  const struct token *tok) noexcept {
  struct asterror *err = asterror_new();
  err->message = msg;
  err->column = tok->column;
  list_push(this->errors, err);
}
