#include "id.h"
#include "lexer.hpp"
#include "tokens.h"
#include "../strings.h"

#define LEXING_STOP() this->column = -1
#define LEXING_STOPPED (this->column < 0)
#define IS_OPERATOR(text, kind) (text.find(kind) == 0)

Ranch::lex::lexer::lexer(std::wstring text) noexcept {
  this->finished = false;
  this->failed = false;
  this->text = text;
  this->column = 0;
}

Ranch::lex::lexer::~lexer() {
  this->text.clear();
  this->column = 0;
  this->failed = false;
  this->finished = false;
}

struct list *Ranch::lex::lexer::lex(void) noexcept {
  this->reset();
  struct list *tokens = list_new(0);
  while (!this->finished && !LEXING_STOPPED) {
    struct token *tok = this->next();
    if (tok->id == ID_IGNORE) { continue; }
    list_push(tokens, tok);
  }
  return tokens;
}

struct token *Ranch::lex::lexer::next(void) noexcept {
  struct token *tok = token_new(ID_NA, NULL, 0);
  this->resume();
  // If ended after resume?
  if (this->finished = this->column >= this->text.length()) {
    tok->id = ID_IGNORE;
    return tok;
  }

  tok->column = this->column + 1; // Increase one for true column value.
  std::wstring text = this->text.substr(this->column);

  if (this->lex_numeric(text, tok))  { goto end; }
  if (this->lex_operator(text, tok)) { goto end; }
  tok->id = ID_IGNORE; // Set token as ignored for not appends to tokens.
  this->error();
  return tok;
end:
  this->column += wcslen(tok->kind);
  return tok;
}

void Ranch::lex::lexer::resume(void) noexcept {
  while (wcs_isspace(this->text[this->column])) { ++this->column; }
}

inline void Ranch::lex::lexer::reset(void) noexcept {
  this->finished = false;
  this->column = 0;
}

bool Ranch::lex::lexer::ended(void) const noexcept {
  return this->finished;
}

bool Ranch::lex::lexer::lex_operator(
  const std::wstring text,
  struct token *tok) noexcept {
  bool state = false;
       if (state = IS_OPERATOR(text, TOKEN_PLUS))          { token_setkind(tok, TOKEN_PLUS); }
  else if (state = IS_OPERATOR(text, TOKEN_MINUS))         { token_setkind(tok, TOKEN_MINUS); }
  else if (state = IS_OPERATOR(text, TOKEN_STAR))          { token_setkind(tok, TOKEN_STAR); }
  else if (state = IS_OPERATOR(text, TOKEN_SLASH))         { token_setkind(tok, TOKEN_SLASH); }
  else if (state = IS_OPERATOR(text, TOKEN_CARET))         { token_setkind(tok, TOKEN_CARET); }
  else if (state = IS_OPERATOR(text, TOKEN_PERCENT))       { token_setkind(tok, TOKEN_PERCENT); }
  else if (state = IS_OPERATOR(text, TOKEN_REVERSE_SLASH)) { token_setkind(tok, TOKEN_REVERSE_SLASH); }
  // If tokenization is success, sets token is an operator.
  if (state) { tok->id = ID_OPERATOR; }
  return state;
}

bool Ranch::lex::lexer::lex_numeric(
  std::wstring text,
  struct token *tok) noexcept {
  if (!wcs_isnumber(text[0])) { return false; }
  uint64_t column = 0;
  bool dotted = false; // For floated values.
  while (++column < text.length()) {
    if (text[column] == TOKEN_DOT[0]) {
      if (dotted) {
        this->column += column; // For dot show to error.
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
  token_setkind(tok, text.substr(0, column).c_str());
  return 1;
}

void Ranch::lex::lexer::error(void) noexcept {
  wprintf(L"Error on lexing;\nUnexpected token: '%lc'\nColumn: %llu\n",
    this->text[this->column], this->column+1);
  this->failed = 1;
  LEXING_STOP();
}

bool Ranch::lex::lexer::fail(void) const noexcept {
  return this->failed;
}
