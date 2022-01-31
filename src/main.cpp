// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <locale>

#include "binopr_events.h"
#include "../include/binopr.h"
#include "../include/messages.h"
#include "../include/ranch.h"
#include "../include/list.h"
#include "../include/strings.h"
#include "../include/value.h"
#include "../include/ast/ast.hpp"
#include "../include/ast/asterror.h"
#include "../include/lex/id.h"
#include "../include/lex/lexer.hpp"
#include "../include/lex/token.h"
#include "../include/lex/tokens.h"
#include "../include/terminal/ansi.h"
#include "../include/terminal/log.h"
#include "../include/terminal/terminal.h"
#include "../include/terminal/commands/commands.h"

// term is a Ranch::terminal instance allocated from heap.
// Allocated at entry point (main) function and deleted here.
// Used to read input from command-line.
struct terminal *term;

void command_help(const wchar_t *cmd);
void command_exit(const wchar_t *cmd);
void command_about(const wchar_t *cmd);
void command_clear(const wchar_t *cmd);
inline void show_about(void);
// Process Ranch terminal command.
void process_command(wchar_t *cmd);
// Loop of the "term" terminal instance.
void term_loop(wchar_t *input);
void parse_expr(std::wstring text);
void free_tokens(struct list *tokens);
void free_operations(struct list *operations);
long long next_operator(const struct list *processes) noexcept;
struct value *compute_expr(struct list *processes) noexcept;
struct value *compute_value_part(struct list *tokens) noexcept;
void erase_processes(struct list *model, long long start, long long end) noexcept;

void command_help(const wchar_t *cmd) {
  if (cmd) {
    LOG_ERROR(ERROR_COMMAND_NOTALONE);
    return;
  }
  printf(
    "COMMAND             DESCRIPTION\n"
    "HELP                Show help\n"
    "EXIT                Exit Ranch\n"
    "ABOUT               Show about of Ranch\n"
    "CLEAR               Clear command-line screen\n"
    "\n");
}

void command_exit(const wchar_t *cmd) {
  if (cmd) {
    LOG_ERROR(ERROR_COMMAND_NOTALONE);
    return;
  }
  exit(EXIT_SUCCESS);
}

void command_about(const wchar_t *cmd) {
  if (cmd) {
    LOG_ERROR(ERROR_COMMAND_NOTALONE);
    return;
  }
  show_about();
}

void command_clear(const wchar_t *cmd) {
  if (cmd) {
    LOG_ERROR(ERROR_COMMAND_NOTALONE);
    return;
  }
  printf(CLEAR_SCREEN POSITION_SET("0", "0"));
}

inline void show_about(void) {
  printf("Ranch CLI Calculator\nVersion: %s\nRelease: %s\n\nCONTRIBUTE\nRepository: %s\n\n",
    RANCH_VERSION, RANCH_RELEASE, RANCH_REPOSITORY);
}

void process_command(wchar_t *cmd) {
  if (wcslen(cmd) == 0) { return; }
  wchar_t *head = command_gethead(cmd);
  if (head) {
    wcslower(head);
    cmd = command_outhead(cmd);
    if (cmd) {
      wchar_t *untrimmed = cmd;
      cmd = wcsltrim(cmd);
      free(untrimmed);
      untrimmed = NULL;
    }
  } else {
    head = cmd;
    cmd = NULL;
  }
       if (wcscmp(head, COMMAND_HELP) == 0)  { command_help(cmd); }
  else if (wcscmp(head, COMMAND_EXIT) == 0)  { command_exit(cmd); }
  else if (wcscmp(head, COMMAND_ABOUT) == 0) { command_about(cmd); }
  else if (wcscmp(head, COMMAND_CLEAR) == 0) { command_clear(cmd); }
  else                                       { LOG_ERROR(ERROR_NOTEXIST_COMMAND); }
  if (cmd) {
    free(head);
    head = NULL;
  }
}

void term_loop(wchar_t *input) {
  if (wcslen(input) == 0) { return; }
  if (input[0] == TOKEN_COLON[0]) {
    wchar_t *cmd = wcsltrim(input+1);
    free(input);
    input = NULL;
    process_command(cmd);
    free(cmd);
    cmd = NULL;
    return;
  }
  parse_expr(std::wstring(input));
  free(input);
  input = NULL;
}

long long next_operator(const struct list *processes) noexcept {
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

struct value *compute_value_part(struct list *tokens) noexcept {
  struct value *val = value_new();
  val->data = wcstod(((struct token*)(tokens->array[0]))->kind, NULL);
  return val;
}

void erase_processes(struct list *model, long long start, long long end) noexcept {
       if (start < 0)    { return; }
  else if (end < start)  { return; }
  else if (start == end) { return; }
  if (end > model->used) { end = model->used; }
  model->used -= end-start;
  for (size_t index = start; index < end; ++index) {
    list_free((struct list*)(model->array[index]));
    model->array[index] = NULL;
  }
}

struct value *compute_expr(struct list *processes) noexcept {
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
      erase_processes(processes, 0, 2);
      goto end;
    }
    if (j == processes->used-1) {
      bop->opr = ((struct token*)(((struct list*)(processes->array[j]))->array[0]))->kind;
      bop->left = compute_value_part((struct list*)(processes->array[j-1]));
      bop->right = val;
      value_repl(val, binopr_solve(bop));
      value_free(bop->left);
      erase_processes(processes, j-1, processes->used);
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
        erase_processes(processes, j+2, j);
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
    erase_processes(processes, j-1, j+2);
    if (processes->used == 1) { break; }
  end:
    j = next_operator(processes);
  }
  if (bop_base.failed) { value_free(val); }
  bopbase_reset();
  return val;
}

void free_tokens(struct list *tokens) {
  for (size_t index = 0; index < tokens->used; ++index) {
    token_free((struct token*)(tokens->array[index]));
  }
  list_free(tokens);
}

void free_operations(struct list *operations) {
  for (size_t index = 0; index < operations->used; ++index) {
    list_free((struct list*)(operations->array[index]));
  }
  list_free(operations);
}

void parse_expr(std::wstring text) {
  Ranch::lex::lexer lexer(text);
  struct list *tokens = lexer.lex();
  if (lexer.fail()) {
    free_tokens(tokens);
    return;
  }
  Ranch::ast::astbuilder ast(tokens);
  struct list *operations = ast.build();
  if (ast.errors->used > 0) {
    free_operations(operations);
    free_tokens(tokens);
    for (size_t index = 0; index < ast.errors->used; ++index) {
      struct asterror *err = (struct asterror*)(ast.errors->array[index]);
      asterror_print(err);
      asterror_free(err);
    }
    return;
  }
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

int main(int argc, char **argv) {
#ifdef __WIN32
  enable_virtual_terminal_processing();
#endif // __WIN32
  setlocale(0x0, ""); // Set locale to all locales.
  wprintf(TITLE_SET(L"Ranch " RANCH_VERSION));
  term = terminal_new();
  term->routine_message = (wchar_t*)(L"Ranch");
  term->sep = (wchar_t*)(TOKEN_GREATER L" ");
  // Prints once Ranch opening message at screen.
  wprintf(L"Ranch CLI Calculator\nVersion " RANCH_VERSION L"\n\n");
  terminal_loop(term, term_loop);
  terminal_free(term);
  return EXIT_SUCCESS;
}
