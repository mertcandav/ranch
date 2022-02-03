// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>

#include "cli.h"
#include "expr.h"
#include "../include/ranch.h"
#include "../include/strings.h"
#include "../include/lex/tokens.h"
#include "../include/terminal/ansi.h"
#include "../include/terminal/terminal.h"

// term is a Ranch::terminal instance allocated from heap.
// Allocated at entry point (main) function and deleted here.
// Used to read input from command-line.
struct terminal *term;

// Loop of the "term" terminal instance.
void term_loop(wchar_t *input);

void term_loop(wchar_t *input) {
  if (wcslen(input) == 0) { return; }
  if (input[0] == TOKEN_COLON[0]) {
    wchar_t *t_input = wcsltrim(input+1);
    free(input);
    input = NULL;
    process_input(t_input);
    free(t_input);
    t_input = NULL;
    return;
  }
  parse_expr(input);
  free(input);
  input = NULL;
}

int main(int argc, char **argv) {
#ifdef __WIN32
  enable_virtual_terminal_processing();
#endif // __WIN32
  setlocale(0x0, "");
  wprintf(TITLE_SET(L"Ranch " RANCH_VERSION));
  // Prints once Ranch opening message at screen.
  wprintf(L"Ranch CLI Calculator\nVersion " RANCH_VERSION L"\n\n");
  term = terminal_new();
  term->routine_message = (wchar_t*)(L"Ranch");
  term->sep = (wchar_t*)(TOKEN_GREATER L" ");
  terminal_loop(term, term_loop);
  terminal_free(term);
  return EXIT_SUCCESS;
}
