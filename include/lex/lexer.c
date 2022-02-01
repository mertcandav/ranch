#include "id.h"
#include "lexer.h"
#include "tokens.h"
#include "../strings.h"
#include "../messages.h"

#define LEXING_STOP() lex->column = -1
#define LEXING_STOPPED (lex->column < 0)
#define IS_OPERATOR(text, kind) (wcsfind(text, kind) == 0)
// Resume from last column.
// Skips if starts with spaces or something ignored values.
#define RESUME() while (wcs_isspace(lex->text[lex->column])) { ++lex->column; }

// Prints error and stops lexing and frees generated tokens.
static inline void error(struct lexer *lex);
static unsigned char lex_numeric(struct lexer *lex, wchar_t *text, struct token *tok);
static unsigned char lex_operator(const wchar_t *text, struct token *tok);

struct lexer *lexer_new(wchar_t *text) {
  struct lexer *lex = (struct lexer*)(malloc(sizeof(struct lexer)));
  if (!lex) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  lex->column = 0;
  lex->failed = 0;
  lex->finished = 0;
  lex->text = text;
  return lex;
}

void lexer_free(struct lexer *lex) {
  free(lex);
  lex = NULL;
}

struct list *lexer_lex(struct lexer *lex) {
  lexer_reset(lex);
  struct list *tokens = list_new(0);
  while (!lex->finished && !LEXING_STOPPED) {
    struct token *tok = lexer_next(lex);
    if (tok->id == ID_IGNORE) { continue; }
    list_push(tokens, tok);
  }
  return tokens;
}

struct token *lexer_next(struct lexer *lex) {
  struct token *tok = token_new(ID_NA, NULL, 0);
  RESUME();
  // If ended after resume?
  if ((lex->finished = lex->column >= wcslen(lex->text))) {
    tok->id = ID_IGNORE;
    return tok;
  }
  tok->column = lex->column + 1; // Increase one for true column value.
  wchar_t *text = wcssub(lex->text, lex->column);
  if (lex_operator(text, tok))      { goto end; }
  if (lex_numeric(lex, text, tok))  { goto end; }
  free(text);
  text = NULL;
  tok->id = ID_IGNORE; // Set token as ignored for not appends to tokens.
  error(lex);
  return tok;
end:
  lex->column += wcslen(tok->kind);
  free(text);
  text = NULL;
  return tok;
}

void lexer_reset(struct lexer *lex) {
  lex->finished = 0;
  lex->column = 0;
}

static unsigned char lex_operator(const wchar_t *text, struct token *tok) {
  unsigned char state = 0;
       if ((state = IS_OPERATOR(text, TOKEN_PLUS)))          { token_setkind(tok, TOKEN_PLUS); }
  else if ((state = IS_OPERATOR(text, TOKEN_MINUS)))         { token_setkind(tok, TOKEN_MINUS); }
  else if ((state = IS_OPERATOR(text, TOKEN_STAR)))          { token_setkind(tok, TOKEN_STAR); }
  else if ((state = IS_OPERATOR(text, TOKEN_SLASH)))         { token_setkind(tok, TOKEN_SLASH); }
  else if ((state = IS_OPERATOR(text, TOKEN_CARET)))         { token_setkind(tok, TOKEN_CARET); }
  else if ((state = IS_OPERATOR(text, TOKEN_PERCENT)))       { token_setkind(tok, TOKEN_PERCENT); }
  else if ((state = IS_OPERATOR(text, TOKEN_REVERSE_SLASH))) { token_setkind(tok, TOKEN_REVERSE_SLASH); }
  // If tokenization is success, sets token is an operator.
  if (state) { tok->id = ID_OPERATOR; }
  return state;
}

static unsigned char lex_numeric(struct lexer *lex,wchar_t *text, struct token *tok) {
  if (!wcs_isnumber(text[0])) { return 0; }
  unsigned long long column = 0;
  unsigned char dotted = 0; // For floated values.
  while (++column < wcslen(text)) {
    if (text[column] == TOKEN_DOT[0]) {
      if (dotted) {
        lex->column += column; // For dot show to error.
        return 0;
      }
      dotted = 1;
      text[column] = L','; // For parsing with floating.
      continue;
    }
    if (wcs_isnumber(text[column])) { continue; }
    break;
  }
  tok->id = ID_VALUE;
  wchar_t *sub = wcsnsub(text, 0, column);
  token_setkind(tok, sub);
  free(sub);
  sub = NULL;
  return 1;
}

static inline void error(struct lexer *lex) {
  wprintf(L"Error on lexing;\nUnexpected token: '%lc'\nColumn: %llu\n",
    lex->text[lex->column], lex->column+1);
  lex->failed = 1;
  LEXING_STOP();
}
