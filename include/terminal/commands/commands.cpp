#include "commands.hpp"

std::wstring Ranch::commands::get_head(const std::wstring cmd) noexcept {
  size_t index = cmd.find(L" ");
  return index == -1 ? cmd : cmd.substr(0, index);
}

std::wstring Ranch::commands::out_head(const std::wstring cmd) noexcept {
  size_t index = cmd.find(L" ");
  return index == -1 ? L"" : cmd.substr(++index);
}
