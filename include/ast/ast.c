#include "ast.h"
#include "../messages.h"
#include "../lex/id.h"
#include "../lex/tokens.h"

static unsigned char require_operator(
  const struct token *tok,
  const size_t index,
  const size_t len);

struct astbuilder *astbuilder_new(struct list *tokens) {
  struct astbuilder *astb = (struct astbuilder*)(calloc(1, sizeof(struct astbuilder)));
  if (!astb) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  astb->errors = list_new(0);
  astb->tokens = tokens;
  return astb;
}

void astbuilder_free(struct astbuilder *astb) {
  if (!astb) { return; }
  list_free(astb->errors);
  free(astb);
  astb = NULL;
}

struct list *astbuilder_build(struct astbuilder *astb) {
  struct list *processes = list_new(1);
  struct list *part = list_new(1);
  unsigned char operator = 0;
  unsigned char value = 0;
  unsigned long long brace_count = 0;
  for (size_t index = 0; index < astb->tokens->used; ++index) {
    struct token *tok = (struct token*)(astb->tokens->array[index]);
    switch (tok->id) {
    case ID_OPERATOR:
      if (!operator && brace_count == 0)
      { astbuilder_pusherr(astb, tok, ERROR_OPERATOR_OVERFLOW); }
      operator = 0;
      value = 1;
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
    case ID_BRACE:
      if (wcscmp(tok->kind, TOKEN_LEFT_PARENTHESES) == 0) { ++brace_count; }
      else                                                { --brace_count; }
    }
    if (index > 0 && brace_count == 0) {
      const struct token *prev_tok = (struct token*)(astb->tokens->array[index-1]);
      if (prev_tok->id == ID_VALUE && tok->id == ID_VALUE)
      { astbuilder_pusherr(astb, tok, ERROR_INVALID_SYNTAX); }
    }
    list_push(part, tok);
    operator = require_operator(tok, index, astb->tokens->used);
    value = 0;
  }
  if (value) {
    struct token *last_tok = (struct token*)(astb->tokens->array[astb->tokens->used-1]);
    astbuilder_pusherr(astb, last_tok, ERROR_OPERATOR_OVERFLOW);
    return processes;
  }
  if (part->used > 0) { list_push(processes, part); }
  return processes;
}

void astbuilder_pusherr(struct astbuilder *astb, struct token *tok, wchar_t *message) {
  struct asterror *err = asterror_new();
  err->message = message;
  err->column = tok->column;
  list_push(astb->errors, err);
}

unsigned char astbuilder_logerr(struct astbuilder *astb, const unsigned char freeErr) {
  if (astb->errors->used < 1) { return 0; }
  for (size_t index = 0; index < astb->errors->used; ++index) {
    struct asterror *err = (struct asterror*)(astb->errors->array[index]);
    asterror_print(err);
    if (freeErr) { asterror_free(err); }
  }
  return 1;
}

static unsigned char require_operator(
  const struct token *tok,
  const size_t index,
  const size_t len) {
  if (tok->id == ID_BRACE && wcscmp(tok->kind, TOKEN_LEFT_PARENTHESES) == 0)
  { return 0; }
  return index < len-1;
}
