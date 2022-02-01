#ifndef __RANCH_COMMANDS_H
#define __RANCH_COMMANDS_H

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Returns head of command.
// If command is only head, returns NULL.
wchar_t *command_gethead(const wchar_t *cmd);
// Returns command without head.
// If command is only head, returns NULL wstring.
wchar_t *command_outhead(const wchar_t *cmd);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_COMMANDS_H
