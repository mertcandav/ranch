// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include <iostream>
#include <locale>
#include <vector>

#include "../include/errors.h"
#include "../include/ranch.h"
#include "../include/ast/ast.hpp"
#include "../include/lex/lexer.hpp"
#include "../include/lex/token.hpp"
#include "../include/lex/tokens.h"
#include "../include/strings/strings.hpp"
#include "../include/terminal/terminal.hpp"
#include "../include/terminal/commands/commands.hpp"

#define IS_COMMAND(cmd) cmd[0] == TOKEN_COLON[0]
#define REMOVE_COMMAND_OPERATOR(cmd) Ranch::strings::wleft_trim(cmd.substr(1))

Ranch::terminal *term;

void command_exit(const std::wstring ns, const std::wstring cmd) noexcept;
void process_command(std::wstring cmd);
void terminal_loop(std::string text);
void parse_expr(std::wstring text);

void command_exit(const std::wstring ns, const std::wstring cmd) noexcept {
  if (cmd != L"") {
    LOG_ERROR(ERR_EXITCOMMAND_NOTALONE)
    return;
  }
  std::exit(0);
}

void process_command(std::wstring cmd) {
  std::wstring ns = Ranch::commands::get_head(cmd);
  cmd = Ranch::commands::out_head(cmd);
  cmd = Ranch::strings::wtrim(cmd);
  if (ns == L"exit") { command_exit(ns, cmd); }
  else               { LOG_ERROR(ERR_NOTEXIST_COMMAND); }
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
  Ranch::ast::astbuilder ast(tokens);
  std::vector<std::vector<Ranch::lex::token>> operations = ast.build();
  if (ast.errors.size() > 0) {
    for (Ranch::ast::error error : ast.errors) { std::wcout << error << std::endl; }
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
  std::setlocale(0x0, "");
  term = new Ranch::terminal();
  term->routine_message = L"Ranch";
  term->sep = TOKEN_GREATER L" ";
  std::wcout << "Ranch CLI Calculator" << std::endl
             << "Version " << RANCH_VERSION << std::endl
             << std::endl;
  term->loop(terminal_loop);
  delete term;
  return 0;
}
