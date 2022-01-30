#ifndef __RANCH_COMMANDS
#define __RANCH_COMMANDS 1

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <wchar.h>

// Returns head of command.
// If command is only head, returns NULL.
wchar_t *command_gethead(const wchar_t *cmd);
// Returns command without head.
// If command is only head, returns NULL wstring.
wchar_t *command_outhead(const wchar_t *cmd);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_COMMANDS
