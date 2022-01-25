// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_TERMINAL
#define __RANCH_TERMINAL 1

#include <iostream>
#include "ansi/ansi.h"

#define LOG_ERROR(msg) std::wcout << COLOR_RED << msg << FONT_RESET << std::endl;

namespace Ranch {
class terminal {
private:
  bool _inloop;
public:
  std::wstring sep;
  std::wstring routine_message;

  terminal() noexcept;
  ~terminal();

  std::wstring once(void) const noexcept;
  void loop(void(*f)(std::wstring cmd));
  inline void stop(void) noexcept;
  constexpr inline bool inloop(void) const noexcept;
};
}

#endif
