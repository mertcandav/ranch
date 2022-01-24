#include "commands.hpp"

std::wstring Ranch::commands::get_head(std::wstring cmd) {
  size_t index = cmd.find(L" ");
  if (index == -1) {
    return cmd;
  }
  return cmd.substr(0, index);
}

std::wstring Ranch::commands::out_head(std::wstring cmd) {
  size_t index = cmd.find(L" ");
  if (index == -1) {
    return L"";
  }
  return cmd.substr(++index);
}
