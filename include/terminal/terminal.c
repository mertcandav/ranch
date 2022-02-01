#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
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
  size_t lenmax = 100;
  wchar_t *line = (wchar_t*)(malloc(lenmax*sizeof(wchar_t)));
  if(!line) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  wchar_t *linep = line;
  size_t len = lenmax;
  wprintf(L"%ls%ls", term->routine_message, term->sep);
  while (1) {
    int c = fgetwc(stdin);
    if (c == EOF || c == '\n') { break; }
    if (--len == 0) {
      len = lenmax;
      wchar_t *linen = (wchar_t*)(realloc(linep, lenmax*=2));
      if (!linen) {
        free(linep);
        wprintf(ERROR_ALLOCATION_FAILED L"\n");
        exit(EXIT_FAILURE);
      }
      line = linen + (line - linep);
      linep = linen;
    }
    *line++ = c;
  }
  *line = '\0';
  return linep;
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
