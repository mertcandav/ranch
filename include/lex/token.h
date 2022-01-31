// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_TOKEN
#define __RANCH_TOKEN 1

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Lex token.
// Kinds are heap-allocated.
typedef struct token {
  unsigned short     id;
  wchar_t            *kind;
  unsigned long long column;
} token;

// Create new token instance allocated from heap.
struct token *token_new(const unsigned short id, const wchar_t *kind, const unsigned long long column);
// Free token instance allocated from heap.
void token_free(struct token *tok);
// Set kind of token.
// Copies to heap specified kind.
// NULL values are frees already allocated kind.
void token_setkind(struct token *tok, const wchar_t *kind);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_TOKEN
