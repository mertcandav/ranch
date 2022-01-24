// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include <iostream>
#include <locale>

#include "../include/ranch.h"
#include "../include/terminal/terminal.hpp"
#include "../include/strings/strings.hpp"
#include "../include/terminal/commands/commands.hpp"

#define IS_COMMAND(cmd) cmd[0] == ':'
#define REMOVE_COMMAND_OPERATOR(cmd) Ranch::strings::wleft_trim(cmd.substr(1))

Ranch::terminal *term;

void command_exit(std::wstring ns, std::wstring cmd) {
  if (cmd != L"") {
    LOG_ERROR(L"exit command is should be single!")
    return;
  }
  std::exit(0);
}

void process_command(std::wstring cmd) {
  std::wstring ns = Ranch::commands::get_head(cmd);
  cmd = Ranch::commands::out_head(cmd);
  cmd = Ranch::strings::wtrim(cmd);
  if (ns == L"exit") {
    command_exit(ns, cmd);
  } else {
    LOG_ERROR(L"There is not such command!");
  }
}

void terminal_loop(std::wstring cmd) {
  cmd = Ranch::strings::wtrim(cmd);
  if (cmd.empty()) { return; }
  if (IS_COMMAND(cmd)) {
    process_command(REMOVE_COMMAND_OPERATOR(cmd));
    return;
  }
}

int main(int argc, char **argv) {
  std::setlocale(0x0, "");
  term = new Ranch::terminal();
  term->routine_message = L"Ranch";
  term->sep = L"> ";
  std::wcout << "Ranch CLI Calculator" << std::endl
             << "Version " << RANCH_VERSION << std::endl
             << std::endl;
  term->loop(terminal_loop);
  delete term;
  return 0;
}
