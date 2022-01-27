// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifdef __WIN32
#include <windows.h>
#endif // __WIN32

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

#ifdef __WIN32
void Ranch::terminal::enable_virtual_terminal_processing(void) noexcept {
  DWORD dwMode = 0;
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE) { return; }
  if (GetConsoleMode(hOut, &dwMode)) {
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
  }
}
#endif // __WIN32
