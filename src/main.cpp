// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include <iostream>
#include <locale>
#include <vector>

#include "../include/binopr.h"
#include "../include/errors.h"
#include "../include/ranch.h"
#include "../include/value.h"
#include "../include/ast/ast.hpp"
#include "../include/ast/asterror.hpp"
#include "../include/lex/id.h"
#include "../include/lex/lexer.hpp"
#include "../include/lex/token.hpp"
#include "../include/lex/tokens.h"
#include "../include/strings/strings.hpp"
#include "../include/terminal/terminal.hpp"
#include "../include/terminal/ansi/ansi.h"
#include "../include/terminal/commands/commands.hpp"

#define IS_COMMAND(cmd) cmd[0] == TOKEN_COLON[0]
#define REMOVE_COMMAND_OPERATOR(cmd) Ranch::strings::wleft_trim(cmd.substr(1))

// term is a Ranch::terminal instance allocated from heap.
// Allocated at entry point (main) function and deleted here.
// Used to read input from command-line.
Ranch::terminal *term;

void command_help(const std::wstring cmd) noexcept;
void command_exit(const std::wstring cmd) noexcept;
void command_about(const std::wstring cmd) noexcept;
void command_clear(const std::wstring cmd) noexcept;
inline void show_about(void) noexcept;
// Process Ranch terminal command.
void process_command(std::wstring cmd);
// Loop of the "term" terminal instance.
void terminal_loop(std::string text);
void parse_expr(std::wstring text);
long long next_operator(Ranch::ast::process_model processes) noexcept;
value *compute_expr(Ranch::ast::process_model processes) noexcept;
value *compute_value_part(Ranch::ast::process_tokens tokens) noexcept;
void erase_processes(Ranch::ast::process_model& model, long long start, long long end) noexcept;

void command_help(const std::wstring cmd) noexcept {
  if (cmd != L"") {
    LOG_ERROR(ERR_COMMAND_NOTALONE);
    return;
  }
  std::wcout
    << "COMMAND             DESCRIPTION" << std::endl
    << "HELP                Show help" << std::endl
    << "EXIT                Exit Ranch" << std::endl
    << "ABOUT               Show about of Ranch" << std::endl
    << "CLEAR               Clear command-line screen" << std::endl
    << std::endl;
}

void command_exit(const std::wstring cmd) noexcept {
  if (cmd != L"") {
    LOG_ERROR(ERR_COMMAND_NOTALONE);
    return;
  }
  std::exit(0);
}

void command_about(const std::wstring cmd) noexcept {
  if (cmd != L"") {
    LOG_ERROR(ERR_COMMAND_NOTALONE);
    return;
  }
  show_about();
}

void command_clear(const std::wstring cmd) noexcept {
  if (cmd != L"") {
    LOG_ERROR(ERR_COMMAND_NOTALONE);
    return;
  }
  std::wcout << CLEAR_SCREEN << POSITION_SET(L"0", L"0");
}

inline void show_about(void) noexcept {
  std::wcout << L"Ranch CLI Calculator" << std::endl
             << L"Version: " << RANCH_VERSION << std::endl
             << L"Release: " << RANCH_RELEASE << std::endl
             << std::endl << "CONTRIBUTE" << std::endl
             << L"Repository: " << RANCH_REPOSITORY
             << std::endl << std::endl;
}

void process_command(std::wstring cmd) {
  if (cmd == L"") { return; }
  std::wstring head = Ranch::commands::get_head(cmd);
  head = Ranch::strings::to_lower(head);
  cmd = Ranch::commands::out_head(cmd);
  cmd = Ranch::strings::wtrim(cmd);
       if (head == COMMAND_HELP)  { command_help(cmd); }
  else if (head == COMMAND_EXIT)  { command_exit(cmd); }
  else if (head == COMMAND_ABOUT) { command_about(cmd); }
  else if (head == COMMAND_CLEAR) { command_clear(cmd); }
  else                            { LOG_ERROR(ERR_NOTEXIST_COMMAND); }
}

void terminal_loop(std::wstring text) {
  text = Ranch::strings::wtrim(text);
  if (text.empty()) { return; }
  if (IS_COMMAND(text)) {
    process_command(REMOVE_COMMAND_OPERATOR(text));
    return;
  }
  parse_expr(text);
}

long long next_operator(Ranch::ast::process_model processes) noexcept {
  long long precedence5 = -1;
  long long precedence4 = -1;
  Ranch::ast::process_model::iterator it = processes.begin();
  Ranch::ast::process_model::iterator end = processes.end();
  for (int index = -1; it < end; ++it) {
    ++index;
         if ((*it).size() != 1)          { continue; }
    else if ((*it)[0].id != ID_OPERATOR) { continue; }
    Ranch::lex::token first = (*it)[0];
         if (first.kind == TOKEN_STAR || first.kind == TOKEN_SLASH || first.kind == TOKEN_PERCENT) { precedence5 = index; }
    else if (first.kind == TOKEN_PLUS || first.kind == TOKEN_MINUS)                                { precedence4 = index; }
  }
       if (precedence5 != -1) { return precedence5; }
  else if (precedence4 != -1) { return precedence4; }
  return -1;
}

value *compute_value_part(Ranch::ast::process_tokens tokens) noexcept {
  value* val = value_new();
  val->data = std::stod(tokens[0].kind);
  return val;
}

void erase_processes(Ranch::ast::process_model& model, long long start, long long end) noexcept {
       if (start < 0)    { return; }
  else if (end < start)  { return; }
  else if (start == end) { return; }
  if (end >= model.size()) { end = model.size(); }
  model.erase(model.begin()+start, model.begin()+end);
}

value *compute_expr(Ranch::ast::process_model processes) noexcept {
  if (!processes.size()) { return nullptr; }
  if (processes.size() == 1) { return compute_value_part(processes[0]); }
  value *val = nullptr;
  binopr *bop = binopr_new();
  long long j = next_operator(processes);
  while (j != -1 && !bop->fail) {
    if (j == 0) {
      bop->left = val;
      bop->opr = (wchar_t*)processes[j][0].kind.c_str();
      bop->right = compute_value_part(processes[j+1]);
      val = binopr_solve(bop);
      erase_processes(processes, 0, 2);
      goto end;
    } else if (j == processes.size()-1) {
      bop->opr = (wchar_t*)processes[j][0].kind.c_str();
      bop->left = compute_value_part(processes[j-1]);
      bop->right = val;
      val = binopr_solve(bop);
      erase_processes(processes, j-1, processes.size());
      goto end;
    } else if (processes[j-1][0].id == ID_OPERATOR && processes[j-1].size() == 1) {
      bop->left = val;
      bop->opr = (wchar_t*)processes[j][0].kind.c_str();
      bop->right = compute_value_part(processes[j+1]);
      val = binopr_solve(bop);
      erase_processes(processes, j+2, j);
      goto end;
    }
    bop->left = compute_value_part(processes[j-1]);
    bop->opr = (wchar_t*)processes[j][0].kind.c_str();
    bop->right = compute_value_part(processes[j+1]);
    {
      value *solved = binopr_solve(bop);
      if (val != nullptr) {
        bop->opr = (wchar_t*)TOKEN_PLUS;
        bop->left = val;
        bop->right = solved;
        val = binopr_solve(bop);
      } else {
        val = solved;
      }
    }
    // Remove computed processes.
    erase_processes(processes, j-1, j+2);
    if (processes.size() == 1) { break; }
  end:
    j = next_operator(processes);
  }
  binopr_free(bop);
  return val;
}

void parse_expr(std::wstring text) {
  Ranch::lex::lexer lexer(text);
  Ranch::ast::process_tokens tokens = lexer.lex();
  if (lexer.fail()) { return; }
  Ranch::ast::astbuilder ast(tokens);
  Ranch::ast::process_model operations = ast.build();
  if (ast.errors.size() > 0) {
    for (Ranch::ast::asterror error : ast.errors) { std::wcout << error << std::endl; }
    return;
  }
  value *val = compute_expr(operations);
  if (val == nullptr) { return; }
  value_print(val);
}

int main(int argc, char **argv) {
#ifdef __WIN32
  Ranch::terminal::enable_virtual_terminal_processing();
#endif // __WIN32
  std::setlocale(0x0, ""); // Set locale to all locales.
  std::wcout << TITLE_SET(L"Ranch " RANCH_VERSION);
  term = new Ranch::terminal();
  term->routine_message = L"Ranch";
  term->sep = TOKEN_GREATER L" ";
  // Prints once Ranch opening message at screen.
  std::wcout << "Ranch CLI Calculator" << std::endl
             << "Version " << RANCH_VERSION << std::endl
             << std::endl;
  term->loop(terminal_loop);
  delete term;
  return 0;
}
