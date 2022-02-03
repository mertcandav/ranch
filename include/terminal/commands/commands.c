#include "commands.h"
#include "../../strings.h"

wchar_t *command_gethead(const wchar_t *cmd) {
  const long long index = wcsfind(cmd, L" ");
  return index == -1 ? NULL : wcsnsub(cmd, 0, (const int)(index));
}

wchar_t *command_outhead(const wchar_t *cmd) {
  const long long index = wcsfind(cmd, L" ");
  return index == -1 ? NULL : wcssub(cmd, index+1);
}
