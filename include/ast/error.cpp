#include "error.hpp"

Ranch::ast::error::error(std::wstring msg, uint64_t column) noexcept {
  this->msg = msg;
  this->column = column;
}

Ranch::ast::error::~error() {
  this->msg.clear();
  this->column = 0;
}
