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
static void free_operations(struct list *operations);
static long long next_operator(const struct list *processes);
static struct value *compute_expr(struct list *processes);
static struct value *compute_value_part(struct list *tokens);

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
             wcscmp(kind, TOKEN_REVERSE_SLASH) == 0) { precedence5 = index; }
    else if (wcscmp(kind, TOKEN_PLUS) == 0  ||
             wcscmp(kind, TOKEN_MINUS) == 0 ||
             wcscmp(kind, TOKEN_CARET) == 0)         { precedence4 = index; }
  }
       if (precedence5 != -1) { return precedence5; }
  else if (precedence4 != -1) { return precedence4; }
  return -1;
}

static struct value *compute_value_part(struct list *tokens) {
  struct value *val = value_new();
  val->data = wcstod(((struct token*)(tokens->array[0]))->kind, NULL);
  return val;
}

static struct value *compute_expr(struct list *processes) {
  if (processes->used == 0) { return NULL; }
  if (processes->used == 1) { return compute_value_part((struct list*)(processes->array[0])); }
  bopbase_setup();
  struct value *val = NULL;
  struct binopr *bop = bop_base.bop;
  long long j = next_operator(processes);
  while (j != -1 && !bop_base.failed) {
    if (j == 0) {
      bop->left = val;
      bop->opr = ((struct token*)(((struct list*)processes->array[0])->array[0]))->kind;
      bop->right = compute_value_part((struct list*)(processes->array[1]));
      value_repl(val, binopr_solve(bop));
      value_free(bop->right);
      list_remrange(processes, 0, 2);
      goto end;
    }
    if (j == processes->used-1) {
      bop->opr = ((struct token*)(((struct list*)(processes->array[j]))->array[0]))->kind;
      bop->left = compute_value_part((struct list*)(processes->array[j-1]));
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
        bop->right = compute_value_part((struct list*)(processes->array[j+1]));
        value_repl(val, binopr_solve(bop));
        value_free(bop->right);
        list_remrange(processes, j+2, j);
        goto end;
      }
    }
    bop->left = compute_value_part((struct list*)(processes->array[j-1]));
    bop->opr = ((struct token*)(((struct list*)(processes->array[j]))->array[0]))->kind;
    bop->right = compute_value_part((struct list*)(processes->array[j+1]));
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
  if (bop_base.failed) { value_free(val); }
  bopbase_reset();
  return val;
}

static void free_tokens(struct list *tokens) {
  for (size_t index = 0; index < tokens->used; ++index) {
    token_free((struct token*)(tokens->array[index]));
  }
  list_free(tokens);
}

static void free_operations(struct list *operations) {
  for (size_t index = 0; index < operations->used; ++index) {
    list_free((struct list*)(operations->array[index]));
  }
  list_free(operations);
}

void parse_expr(wchar_t *text) {
  struct lexer *lex = lexer_new(text);
  struct list *tokens = lexer_lex(lex);
  lexer_free(lex);
  if (lex->failed) {
    free_tokens(tokens);
    return;
  }
  struct astbuilder *astb = astbuilder_new(tokens);
  struct list *operations = astbuilder_build(astb);
  if (astb->errors->used > 0) {
    free_operations(operations);
    free_tokens(tokens);
    for (size_t index = 0; index < astb->errors->used; ++index) {
      struct asterror *err = (struct asterror*)(astb->errors->array[index]);
      asterror_print(err);
      asterror_free(err);
    }
    astbuilder_free(astb);
    return;
  }
  astbuilder_free(astb);
  struct value *val = compute_expr(operations);
  free_operations(operations);
  free_tokens(tokens);
  if (!val) {
    if (bop_base.failed) { LOG_ERROR(ERROR_COMPUTED_FAILED); }
    return;
  }
  value_print(val);
  value_free(val);
}
