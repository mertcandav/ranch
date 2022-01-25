#include "id.h"
#include "lexer.hpp"
#include "../strings/strings.hpp"

#define IS_OPERATOR(text, kind) text.find(kind) == 0
#define MAKE_OPERATOR(kind, column) Ranch::lex::token(ID_OPERATOR, kind, column)

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
  while (!this->finished && this->column != -1) {
    Ranch::lex::token token = this->next();
    if (token.id == ID_IGNORE) { continue; }
    tokens.push_back(token);
  }
  return tokens;
}

Ranch::lex::token Ranch::lex::lexer::next(void) noexcept {
  Ranch::lex::token token;
  this->resume();
  if (this->column >= this->text.length()) {
    this->finished = true;
    token.id = ID_IGNORE;
    return token;
  }
  token.column = this->column;
  std::wstring text = this->text.substr(this->column);
  if (this->lex_numeric(text, &token))  { goto end; }
  if (this->lex_operator(text, &token)) { goto end; }
  token.id = ID_IGNORE;
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
       if (state = IS_OPERATOR(text, L"+")) { *token = MAKE_OPERATOR(L"+", this->column); }
  else if (state = IS_OPERATOR(text, L"-")) { *token = MAKE_OPERATOR(L"-", this->column); }
  else if (state = IS_OPERATOR(text, L"*")) { *token = MAKE_OPERATOR(L"*", this->column); }
  else if (state = IS_OPERATOR(text, L"/")) { *token = MAKE_OPERATOR(L"/", this->column); }
  return state;
}

bool Ranch::lex::lexer::lex_numeric(
  const std::wstring text,
  Ranch::lex::token *token) noexcept {
  if (!Ranch::strings::is_number(text[0])) { return false; }
  uint64_t column = 0;
  while (++column < text.length() && Ranch::strings::is_number(text[column]));
  token->id = ID_VALUE;
  token->kind = text.substr(0, column);
  return true;
}

void Ranch::lex::lexer::error(void) noexcept {
  std::wcout << L"Error on lexing;" << std::endl
             << L"Unexpected token: " << this->text[this->column];
  this->column = -1; // For stop.
}
