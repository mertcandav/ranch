#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "id.h"
#include "token.h"
#include "../messages.h"

struct token *token_new(
  const unsigned short id,
  const wchar_t *kind,
  const unsigned long long column) {
  struct token *tok = (struct token*)(calloc(1, sizeof(struct token)));
  if (!tok) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  tok->column = column;
  tok->id = id;
  if (kind) { token_setkind(tok, kind); }
  return tok;
}

void token_free(struct token *tok) {
  if (!tok) { return; }
  token_setkind(tok, NULL);
  free(tok);
  tok = NULL;
}

void token_setkind(struct token *tok, const wchar_t *kind) {
  if (!kind) {
    free(tok->kind);
    tok->kind = NULL;
    return;
  }
  free(tok->kind);
  tok->kind = NULL;
  tok->kind = wcsdup(kind);
}
