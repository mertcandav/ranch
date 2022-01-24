// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include "terminal.hpp"

Ranch::terminal::terminal() {}
Ranch::terminal::~terminal() {}

std::wstring Ranch::terminal::once() {
  std::wcout << this->routine_message;
  std::wcout << this->sep;
  std::wstring input;
  std::getline(std::wcin, input);
  return input;
}

void Ranch::terminal::loop(void(*f)(std::wstring cmd)) {
  if (f == nullptr) {
    throw std::invalid_argument("loop function is was nullptr");
  }
  this->_inloop = true;
  while (this->_inloop) {
    std::wstring cmd = this->once();
    (*f)(cmd);
  }
}

inline void Ranch::terminal::stop() noexcept {
  this->_inloop = false;
}

inline bool Ranch::terminal::inloop() noexcept {
  return this->_inloop;
}
