// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include "terminal.hpp"

Ranch::terminal::terminal() noexcept {
  this->routine_message = L"";
  this->sep = L"";
  this->_inloop = false;
}

Ranch::terminal::~terminal() { }

std::wstring Ranch::terminal::once(void) const noexcept {
  std::wcout << this->routine_message;
  std::wcout << this->sep;
  std::wstring input;
  std::getline(std::wcin, input);
  return input;
}

void Ranch::terminal::loop(void(*f)(std::wstring cmd)) {
  if (f == nullptr) { throw std::invalid_argument("loop function is was nullptr"); }
  this->_inloop = true;
  while (this->_inloop) { (*f)(this->once()); }
}

inline void Ranch::terminal::stop(void) noexcept {
  this->_inloop = false;
}

constexpr inline bool Ranch::terminal::inloop(void) const noexcept {
  return this->_inloop;
}
