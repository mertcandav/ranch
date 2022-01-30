#include <stdlib.h>
#include <stdio.h>
#ifdef __WIN32
#include <windows.h>
#endif // __WIN32

#include "terminal.h"
#include "../messages.h"

#ifdef __WIN32
void enable_virtual_terminal_processing(void) {
  DWORD dwMode = 0;
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE) { return; }
  if (GetConsoleMode(hOut, &dwMode)) {
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
  }
}
#endif // __WIN32

struct terminal *terminal_new(void) {
  struct terminal *term = (struct terminal*)calloc(1, sizeof(struct terminal));
  if (!term) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  term->inloop = 0;
  term->routine_message = NULL;
  term->sep = NULL;
  return term;
}

void terminal_free(struct terminal *term) {
  free(term);
  term = NULL;
}

wchar_t *terminal_once(struct terminal *term) {
  wprintf(L"%ls%ls", term->routine_message, term->sep);
  wchar_t *input = (wchar_t*)(malloc(3000*sizeof(wchar_t)));
  scanf("%ls", input);
  return input;
}

void terminal_loop(struct terminal *term, void(*f)(wchar_t *input)) {
  if (!f) {
    printf("loop function is was null\n");
    exit(EXIT_FAILURE);
  }
  term->inloop = 1;
  while (term->inloop) { (*f)(terminal_once(term)); }
}

void terminal_stop(struct terminal *term) {
  term->inloop = 0;
}
