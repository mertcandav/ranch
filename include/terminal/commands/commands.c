#include "commands.h"
#include "../../strings/strings.h"

wchar_t *command_gethead(const wchar_t *cmd) {
  const long long index = wcsfind(cmd, L" ");
  if (index == -1) { return NULL; }
  return wcsnsub(cmd, 0, (const int)(index));
}

wchar_t *command_outhead(const wchar_t *cmd) {
  const long long index = wcsfind(cmd, L" ");
  if (index == -1) { return NULL; }
  return wcssub(cmd, index+1);
}
