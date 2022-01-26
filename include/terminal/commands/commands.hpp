#ifndef __RANCH_COMMANDS
#define __RANCH_COMMANDS 1

#include <iostream>

namespace Ranch::commands {
// Returns head of command.
// If command is only head, returns self.
std::wstring get_head(const std::wstring cmd) noexcept;
// Returns command without head.
// If command is only head, returns empty wstring.
std::wstring out_head(const std::wstring cmd) noexcept;
}

#endif
