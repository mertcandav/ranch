#include "expr.h"
#include "binopr_events.h"
#include "../include/binopr.h"
#include "../include/messages.h"
#include "../include/ranch.h"
#include "../include/list.h"
#include "../include/strings.h"
#include "../include/value.h"
#include "../include/ast/ast.h"
#include "../include/ast/asterror.h"
#include "../include/lex/id.h"
#include "../include/lex/lexer.h"
#include "../include/lex/token.h"
#include "../include/lex/tokens.h"
#include "../include/terminal/log.h"

static void free_tokens(struct list *tokens);
static void free_processes(struct list *operations);
static long long next_operator(const struct list *processes);
static struct value *compute_expr(struct list *processes);
static struct value *compute_expr_part(struct list *tokens);
static struct value *compute_single_value_part(struct token *tok);
static struct value *compute_parentheses_expr_part(struct list *tokens);
static inline void throw_parse_error(struct token *tok, const wchar_t *message);
static inline value *compute_expr_from_tokens(struct list* tokens);

static inline void throw_parse_error(struct token *tok, const wchar_t *message) {
  LOG_ERROR(L"PARSER ERROR: ");
  wprintf(L"%ls\nColumn: %llu\n", message, tok->column);
}

static long long next_operator(const struct list *processes) {
  long long precedence5 = -1;
  long long precedence4 = -1;
  for (size_t index = 0; index < processes->used; ++index) {
    const struct list *lst = (struct list*)(processes->array[index]);
    if (lst->used != 1) { continue; }
    const struct token *first_tok = (struct token*)(lst->array[0]);
    if (first_tok->id != ID_OPERATOR) { continue; }
    const wchar_t *kind = first_tok->kind;
         if (wcscmp(kind, TOKEN_STAR) == 0    ||
             wcscmp(kind, TOKEN_SLASH) == 0   ||
             wcscmp(kind, TOKEN_PERCENT) == 0 ||
             wcscmp(kind, TOKEN_REVERSE_SLASH) == 0) { precedence5 = index; break; }
    else if (wcscmp(kind, TOKEN_PLUS) == 0  ||
             wcscmp(kind, TOKEN_MINUS) == 0 ||
             wcscmp(kind, TOKEN_CARET) == 0)         { precedence4 = index; }
  }
       if (precedence5 != -1) { return precedence5; }
  else if (precedence4 != -1) { return precedence4; }
  return -1;
}

static struct value *compute_single_value_part(struct token *tok) {
  struct value *val = value_new();
  val->data  = wcstod(tok->kind, NULL);
  return val;
}

static struct value *compute_parentheses_expr_part(struct list *tokens) {
  unsigned int brace_count = 0;
  for (size_t j = tokens->used-1; j >= 0; --j) {
    struct token *tok = (struct token*)(tokens->array[j]);
    if (tok->id != ID_BRACE) { continue; }
    if (wcscmp(tok->kind, TOKEN_RIGHT_PARENTHESES) == 0) { ++brace_count; }
    else                                                 { --brace_count; }
    if (brace_count > 0) { continue; }
    if (j+2 == tokens->used) { // () ?
      throw_parse_error(tokens->array[tokens->used-1], ERROR_PARENTHESES_EMPTY);
      return NULL;
    }
    break;
  }
  if (brace_count != 0) {
    throw_parse_error(tokens->array[tokens->used-1], ERROR_INVALID_SYNTAX);
    return NULL;
  }
  struct token *tok = (struct token*)(tokens->array[0]);
  list_remrange(tokens, 0, 1);
  list_remrange(tokens, tokens->used-1, 1);
  if (tokens->used == 0) {
    throw_parse_error(tok, ERROR_INVALID_SYNTAX);
    return NULL;
  }
  return compute_expr_from_tokens(tokens);
}

static struct value *compute_expr_part(struct list *tokens) {
  struct token *first_tok = (struct token*)(tokens->array[0]);
  if (tokens->used == 1) { return compute_single_value_part(first_tok); }
  struct token *end_tok = (struct token*)(tokens->array[tokens->used-1]);
  if (wcscmp(end_tok->kind, TOKEN_RIGHT_PARENTHESES) == 0)
  { return compute_parentheses_expr_part(tokens); }
  else
  { throw_parse_error(end_tok, ERROR_INVALID_SYNTAX); }
  return NULL;
}

static inline value *compute_expr_from_tokens(struct list* tokens) {
  struct astbuilder *astb = astbuilder_new(tokens);
  struct list* processes = astbuilder_build(astb);
  if (astbuilder_logerr(astb, 1)) {
    free_processes(processes);
    astbuilder_free(astb);
    return NULL;
  }
  astbuilder_free(astb);
  struct value *val = compute_expr(processes);
  free_processes(processes);
  return val;
}

static struct value *compute_expr(struct list *processes) {
  if (processes->used == 0) { return NULL; }
  if (processes->used == 1)
  { return compute_expr_part((struct list*)(processes->array[0])); }
  struct value *val = NULL;
  struct binopr *bop = bopbase_newbop();
  long long j = next_operator(processes);
  while (j != -1 && !bopbase.failed) {
    if (j == 0) {
      bop->left = val;
      bop->opr = ((struct token*)(((struct list*)processes->array[0])->array[0]))->kind;
      bop->right = compute_expr_part((struct list*)(processes->array[1]));
      value_repl(val, binopr_solve(bop));
      value_free(bop->right);
      list_remrange(processes, 0, 2);
      goto end;
    }
    if (j == processes->used-1) {
      bop->opr = ((struct token*)(((struct list*)(processes->array[j]))->array[0]))->kind;
      bop->left = compute_expr_part((struct list*)(processes->array[j-1]));
      bop->right = val;
      value_repl(val, binopr_solve(bop));
      value_free(bop->left);
      list_remrange(processes, j-1, processes->used);
      goto end;
    }
    {
      const struct list *prev_list = (struct list*)(processes->array[j-1]);
      if (((struct token*)(prev_list->array[0]))->id == ID_OPERATOR && prev_list->used == 1) {
        bop->left = val;
        bop->opr = ((struct token*)(((struct list*)processes->array[j])->array[0]))->kind;
        bop->right = compute_expr_part((struct list*)(processes->array[j+1]));
        value_repl(val, binopr_solve(bop));
        value_free(bop->right);
        list_remrange(processes, j+2, j);
        goto end;
      }
    }
    bop->left = compute_expr_part((struct list*)(processes->array[j-1]));
    bop->opr = ((struct token*)(((struct list*)(processes->array[j]))->array[0]))->kind;
    bop->right = compute_expr_part((struct list*)(processes->array[j+1]));
    {
      struct value *solved = binopr_solve(bop);
      value_free(bop->left);
      value_free(bop->right);
      if (val) {
        bop->opr = (wchar_t*)TOKEN_PLUS;
        bop->left = val;
        bop->right = solved;
        value_repl(val, binopr_solve(bop));
        value_free(solved);
      } else {
        val = solved;
      }
    }
    list_remrange(processes, j-1, j+2);
    if (processes->used == 1) { break; }
  end:
    j = next_operator(processes);
  }
  if (bopbase.failed) { value_free(val); }
  binopr_free(bop);
  return val;
}

static void free_tokens(struct list *tokens) {
  for (size_t index = 0; index < tokens->used; ++index)
  { token_free((struct token*)(tokens->array[index])); }
  list_free(tokens);
}

static void free_processes(struct list *processes) {
  for (size_t index = 0; index < processes->used; ++index)
  { list_free((struct list*)(processes->array[index])); }
  list_free(processes);
}

void parse_expr(wchar_t *text) {
  struct lexer *lex = lexer_new(text);
  struct list *tokens = lexer_lex(lex);
  if (lex->failed) {
    lexer_free(lex);
    free_tokens(tokens);
    return;
  }
  lexer_free(lex);
  struct astbuilder *astb = astbuilder_new(tokens);
  struct list *processes = astbuilder_build(astb);
  if (astbuilder_logerr(astb, 1)) {
    free_processes(processes);
    free_tokens(tokens);
    astbuilder_free(astb);
    return;
  }
  astbuilder_free(astb);
  bopbase_reset();
  struct value *val = compute_expr(processes);
  free_processes(processes);
  free_tokens(tokens);
  if (bopbase.failed) { LOG_ERROR(ERROR_COMPUTED_FAILED); }
  if (val) {
    value_print(val);
    value_free(val);
  }
}
