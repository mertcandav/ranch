// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_TERMINAL
#define __RANCH_TERMINAL 1

#include <iostream>
#include "ansi/ansi.h"

// Prints error with new line to command-line.
#define LOG_ERROR(msg) std::wcout << COLOR_RED << msg << FONT_RESET << std::endl

namespace Ranch {
class terminal {
private:
  bool _inloop;
public:

  // Prints before reads each command-line input.
  std::wstring routine_message;
  // Seperator for routine message.
  // Prints after printed the routine message.
  std::wstring sep;

  terminal() noexcept;
  ~terminal();

  // Returns once input from command-line with terminal configs.
  std::wstring once(void) const noexcept;
  // Into the command-line input loop with specified function.
  // The parameter of specified function, takes input from command-line
  // with self "once" method.
  void loop(void(*f)(std::wstring cmd));
  // Stops loop if looping.
  inline void stop(void) noexcept;
  constexpr inline bool inloop(void) const noexcept;
};
}

#endif
