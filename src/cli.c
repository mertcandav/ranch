#include <stdlib.h>
#include <stdio.h>

#include "cli.h"
#include "../include/ranch.h"
#include "../include/messages.h"
#include "../include/strings.h"
#include "../include/terminal/ansi.h"
#include "../include/terminal/log.h"
#include "../include/terminal/commands/commands.h"

static void command_help(const wchar_t *cmd);
static void command_exit(const wchar_t *cmd);
static void command_about(const wchar_t *cmd);
static void command_clear(const wchar_t *cmd);
static inline void show_about(void);

static void command_help(const wchar_t *cmd) {
  if (cmd) {
    LOG_ERROR(ERROR_COMMAND_NOTALONE);
    return;
  }
  printf(
    "COMMAND             DESCRIPTION\n"
    "HELP                Show help\n"
    "EXIT                Exit Ranch\n"
    "ABOUT               Show about of Ranch\n"
    "CLEAR               Clear command-line screen\n"
    "\n");
}

static void command_exit(const wchar_t *cmd) {
  if (cmd) {
    LOG_ERROR(ERROR_COMMAND_NOTALONE);
    return;
  }
  exit(EXIT_SUCCESS);
}

static void command_about(const wchar_t *cmd) {
  if (cmd) {
    LOG_ERROR(ERROR_COMMAND_NOTALONE);
    return;
  }
  show_about();
}

static void command_clear(const wchar_t *cmd) {
  if (cmd) {
    LOG_ERROR(ERROR_COMMAND_NOTALONE);
    return;
  }
  wprintf(CLEAR_SCREEN POSITION_SET(L"0", L"0"));
}

static inline void show_about(void) {
  printf("Ranch CLI Calculator\nVersion: %s\nRelease: %s\n\nCONTRIBUTE\nRepository: %s\n\n",
    RANCH_VERSION, RANCH_RELEASE, RANCH_REPOSITORY);
}

void process_input(wchar_t *cmd) {
  if (wcslen(cmd) == 0) { return; }
  wchar_t *head = command_gethead(cmd);
  if (head) {
    wcslower(head);
    cmd = command_outhead(cmd);
    if (cmd) {
      wchar_t *untrimmed = cmd;
      cmd = wcsltrim(cmd);
      free(untrimmed);
      untrimmed = NULL;
    }
  } else {
    head = cmd;
    cmd = NULL;
  }
       if (wcscmp(head, COMMAND_HELP) == 0)  { command_help(cmd); }
  else if (wcscmp(head, COMMAND_EXIT) == 0)  { command_exit(cmd); }
  else if (wcscmp(head, COMMAND_ABOUT) == 0) { command_about(cmd); }
  else if (wcscmp(head, COMMAND_CLEAR) == 0) { command_clear(cmd); }
  else                                       { LOG_ERROR(ERROR_NOTEXIST_COMMAND); }
  if (cmd) {
    free(head);
    head = NULL;
  }
}
