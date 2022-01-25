#include "id.h"
#include "token.hpp"

Ranch::lex::token::token() noexcept {
  this->id = ID_NA;
  this->kind = L"";
  this->column = 0;
}

Ranch::lex::token::token(
  uint16_t id,
  std::wstring kind,
  uint64_t column) noexcept {
  this->id = id;
  this->kind = kind;
  this->column = column;
}

Ranch::lex::token::~token() {
  this->kind.clear();
  this->id = 0;
  this->column = 0;
}
