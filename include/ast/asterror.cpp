#include "asterror.hpp"

Ranch::ast::asterror::asterror(std::wstring msg, uint64_t column) noexcept {
  this->msg = msg;
  this->column = column;
}

Ranch::ast::asterror::~asterror() {
  this->msg.clear();
  this->column = 0;
}
