#include "id.h"
#include "lexer.hpp"
#include "tokens.h"
#include "../strings/strings.hpp"

#define LEXING_STOP this->column = -1
#define LEXING_STOPPED (this->column < 0)
#define IS_OPERATOR(text, kind) (text.find(kind) == 0)

Ranch::lex::lexer::lexer(std::wstring text) noexcept {
  this->finished = false;
  this->text = text;
  this->column = 0;
}

Ranch::lex::lexer::~lexer() {
  this->text.clear();
  this->column = 0;
  this->finished = false;
}

std::vector<Ranch::lex::token> Ranch::lex::lexer::lex(void) noexcept {
  this->reset();
  std::vector<Ranch::lex::token> tokens;
  while (!this->finished && !LEXING_STOPPED) {
    Ranch::lex::token token = this->next();
    if (token.id == ID_IGNORE) { continue; }
    tokens.push_back(token);
  }
  return tokens;
}

Ranch::lex::token Ranch::lex::lexer::next(void) noexcept {
  Ranch::lex::token token;
  this->resume();
  // If ended after resume?
  if (this->column >= this->text.length()) {
    this->finished = true;
    token.id = ID_IGNORE;
    return token;
  }

  token.column = this->column + 1; // Increase one for true column value.
  std::wstring text = this->text.substr(this->column);

  if (this->lex_numeric(text, &token))  { goto end; }
  if (this->lex_operator(text, &token)) { goto end; }
  token.id = ID_IGNORE; // Set token as ignored for not appends to tokens.
  this->error();
end:
  this->column += token.kind.length();
  return token;
}

void Ranch::lex::lexer::resume(void) noexcept {
  while (Ranch::strings::is_space(this->text[this->column])) { ++this->column; }
}

inline void Ranch::lex::lexer::reset(void) noexcept {
  this->finished = false;
  this->column = 0;
}

constexpr inline bool Ranch::lex::lexer::ended(void) const noexcept {
  return this->finished;
}

bool Ranch::lex::lexer::lex_operator(
  const std::wstring text,
  Ranch::lex::token *token) noexcept {
  bool state = false;
       if (state = IS_OPERATOR(text, TOKEN_PLUS))  { token->kind = TOKEN_PLUS; }
  else if (state = IS_OPERATOR(text, TOKEN_MINUS)) { token->kind = TOKEN_MINUS; }
  else if (state = IS_OPERATOR(text, TOKEN_STAR))  { token->kind = TOKEN_STAR; }
  else if (state = IS_OPERATOR(text, TOKEN_SLASH)) { token->kind = TOKEN_SLASH; }
  // If tokenization is success, set token is an as operator.
  if (state) { token->id = ID_OPERATOR; }
  return state;
}

bool Ranch::lex::lexer::lex_numeric(
  const std::wstring text,
  Ranch::lex::token *token) noexcept {
  if (!Ranch::strings::is_number(text[0])) { return false; }
  uint64_t column = 0;
  bool dotted = false; // For floated values.
  while (++column < text.length()) {
    if (text[column] == TOKEN_DOT[0]) {
      if (dotted) {
        this->column += column; // For dot show to error.
        return false;
      }
      dotted = true;
      continue;
    }
    if (Ranch::strings::is_number(text[column])) { continue; }
    break;
  }
  token->id = ID_VALUE;
  token->kind = text.substr(0, column);
  return true;
}

void Ranch::lex::lexer::error(void) noexcept {
  std::wcout << L"Error on lexing;" << std::endl
             << L"Unexpected token: '" << this->text[this->column] << L"'" << std::endl
             << L"Column: " << this->column << std::endl;
  LEXING_STOP;
}
