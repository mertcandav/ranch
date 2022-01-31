// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <locale>
#include <vector>

#include "binopr_events.h"
#include "../include/binopr.h"
#include "../include/messages.h"
#include "../include/ranch.h"
#include "../include/value.h"
#include "../include/ast/ast.hpp"
#include "../include/ast/asterror.h"
#include "../include/lex/id.h"
#include "../include/lex/lexer.hpp"
#include "../include/lex/token.h"
#include "../include/lex/tokens.h"
#include "../include/strings/strings.h"
#include "../include/strings/strings.hpp"
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
void free_tokens(Ranch::ast::process_tokens &tokens) noexcept;
long long next_operator(Ranch::ast::process_model processes) noexcept;
struct value *compute_expr(Ranch::ast::process_model processes) noexcept;
struct value *compute_value_part(Ranch::ast::process_tokens tokens) noexcept;
void erase_processes(Ranch::ast::process_model &model, long long start, long long end) noexcept;

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
  std::wstring cmd(input);
  free(input);
  input = NULL;
  if (cmd.size() == 0) { return; }
  if (cmd[0] == TOKEN_COLON[0]) {
    process_command((wchar_t*)(Ranch::strings::wleft_trim(cmd.substr(1)).c_str()));
    return;
  }
  parse_expr(cmd);
}

long long next_operator(Ranch::ast::process_model processes) noexcept {
  long long precedence5 = -1;
  long long precedence4 = -1;
  Ranch::ast::process_model::iterator it = processes.begin();
  Ranch::ast::process_model::iterator end = processes.end();
  for (int index = -1; it < end; ++it) {
    ++index;
         if ((*it).size() != 1)          { continue; }
    else if ((*it)[0]->id != ID_OPERATOR) { continue; }
    const wchar_t *first = (*it)[0]->kind;
         if (wcscmp(first, TOKEN_STAR) == 0    ||
             wcscmp(first, TOKEN_SLASH) == 0   ||
             wcscmp(first, TOKEN_PERCENT) == 0 ||
             wcscmp(first, TOKEN_REVERSE_SLASH) == 0) { precedence5 = index; }
    else if (wcscmp(first, TOKEN_PLUS) == 0  ||
             wcscmp(first, TOKEN_MINUS) == 0 ||
             wcscmp(first, TOKEN_CARET) == 0)         { precedence4 = index; }
  }
       if (precedence5 != -1) { return precedence5; }
  else if (precedence4 != -1) { return precedence4; }
  return -1;
}

struct value *compute_value_part(Ranch::ast::process_tokens tokens) noexcept {
  struct value *val = value_new();
  val->data = wcstod(tokens[0]->kind, NULL);
  return val;
}

void erase_processes(Ranch::ast::process_model &model, long long start, long long end) noexcept {
       if (start < 0)    { return; }
  else if (end < start)  { return; }
  else if (start == end) { return; }
  if (end >= model.size()) { end = model.size(); }
  model.erase(model.begin()+start, model.begin()+end);
}

struct value *compute_expr(Ranch::ast::process_model processes) noexcept {
  if (processes.size() == 0) { return NULL; }
  if (processes.size() == 1) { return compute_value_part(processes[0]); }
  bopbase_setup();
  struct value *val = NULL;
  struct binopr *bop = bop_base.bop;
  long long j = next_operator(processes);
  while (j != -1 && !bop_base.failed) {
    if (j == 0) {
      bop->left = val;
      bop->opr = processes[j][0]->kind;
      bop->right = compute_value_part(processes[j+1]);
      value_repl(val, binopr_solve(bop));
      value_free(bop->right);
      erase_processes(processes, 0, 2);
      goto end;
    } else if (j == processes.size()-1) {
      bop->opr = processes[j][0]->kind;
      bop->left = compute_value_part(processes[j-1]);
      bop->right = val;
      value_repl(val, binopr_solve(bop));
      value_free(bop->left);
      erase_processes(processes, j-1, processes.size());
      goto end;
    } else if (processes[j-1][0]->id == ID_OPERATOR && processes[j-1].size() == 1) {
      bop->left = val;
      bop->opr = processes[j][0]->kind;
      bop->right = compute_value_part(processes[j+1]);
      value_repl(val, binopr_solve(bop));
      value_free(bop->right);
      erase_processes(processes, j+2, j);
      goto end;
    }
    bop->left = compute_value_part(processes[j-1]);
    bop->opr = processes[j][0]->kind;
    bop->right = compute_value_part(processes[j+1]);
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
    if (processes.size() == 1) { break; }
  end:
    j = next_operator(processes);
  }
  if (bop_base.failed) { value_free(val); }
  bopbase_reset();
  return val;
}

void free_tokens(Ranch::ast::process_tokens &tokens) noexcept {
  Ranch::ast::process_tokens::iterator it = tokens.begin();
  const Ranch::ast::process_tokens::iterator end = tokens.end();
  for (; it < end; ++it) { token_free(*it); }
}

void parse_expr(std::wstring text) {
  Ranch::lex::lexer lexer(text);
  Ranch::ast::process_tokens tokens = lexer.lex();
  if (lexer.fail()) {
    free_tokens(tokens);
    return;
  }
  Ranch::ast::astbuilder ast(tokens);
  Ranch::ast::process_model operations = ast.build();
  if (ast.errors.size() > 0) {
    free_tokens(tokens);
    for (struct asterror error : ast.errors) { asterror_print(error); }
    return;
  }
  struct value *val = compute_expr(operations);
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
  return 0;
}
