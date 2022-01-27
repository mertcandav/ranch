// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include <iostream>
#include <locale>
#include <vector>

#include "../include/errors.h"
#include "../include/ranch.h"
#include "../include/ast/ast.hpp"
#include "../include/ast/asterror.hpp"
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
// Parses Ranch expressions.
void parse_expr(std::wstring text);

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

void parse_expr(std::wstring text) {
  Ranch::lex::lexer lexer(text);
  std::vector<Ranch::lex::token> tokens = lexer.lex();
  if (lexer.fail()) { return; }
  Ranch::ast::astbuilder ast(tokens);
  std::vector<std::vector<Ranch::lex::token>> operations = ast.build();
  if (ast.errors.size() > 0) {
    for (Ranch::ast::asterror error : ast.errors) { std::wcout << error << std::endl; }
    return;
  }
  for (std::vector<Ranch::lex::token> tokens : operations) {
    std::wcout << L"[ ";
    for (Ranch::lex::token token : tokens) {
      std::wcout << token.kind << L" ";
    }
    std::wcout << L"] ";
  }
  std::wcout << std::endl;
}

int main(int argc, char **argv) {
#ifdef __WIN32
  Ranch::terminal::enable_virtual_terminal_processing();
#endif // __WIN32
  std::setlocale(0x0, ""); // Set locale to all locales.
  std::wcout << TITLE_SET(L"Ranch");
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
