// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_LEXER_H
#define __RANCH_LEXER_H

#include "token.h"
#include "../list.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Lexer.
typedef struct lexer {
  unsigned char      failed;
  unsigned char      finished;
  long long          parentheses;
  wchar_t            *text;
  // Current lexer column.
  // This is a index value actually.
  unsigned long long column;
} lexer;

// Create new lexer instance allocated from heap.
struct lexer *lexer_new(wchar_t *text);
// Free lexer instance allocated from heap.
void lexer_free(struct lexer *lex);
// Resets everything, returns to start.
void lexer_reset(struct lexer *lex);
// Lexs and returns all tokens of text.
// If exist any tokenization error, prints and stops.
struct list *lexer_lex(struct lexer *lex);
// Lexs and returns next token.
struct token *lexer_next(struct lexer *lex);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_LEXER_H
