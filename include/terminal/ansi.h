// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

// This header includes ANSI escape codes.

#ifndef __RANCH_ANSI_H
#define __RANCH_ANSI_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define CLEAR_SCREEN L"\e[1;1H\e[2J"
#define CLEAR_SCREEN_CB L"\e[1;1H\e[1J"
#define CLEAR_SCREEN_CE L"\e[1;1H\e[0J"

#define CLEAR_LINE L"\e[1;1H\e[2K"
#define CLEAR_LINE_CB L"\e[1;1H\e[1K"
#define CLEAR_LINECE L"\e[1;1H\e[0K"

#define SHOW_CURSOR L"\033[?25h"
#define HIDE_CURSOR L"\033[?25l"

#define FONT_RESET L"\033[0m"
#define FONT_BOLD L"\u001b[1m"
#define FONT_UNDERLINE L"\e[1;1H\e[4m"
#define FONT_REVERSED L"\e[1;1H\e[7m"

#define COLOR_BLACK L"\033[30m"
#define COLOR_RED L"\033[31m"
#define COLOR_GREEN L"\033[32m"
#define COLOR_YELLOW L"\033[33m"
#define COLOR_BLUE L"\033[34m"
#define COLOR_MAGENTA L"\033[35m"
#define COLOR_CYAN L"\033[36m"
#define COLOR_WHITE L"\033[37m"
#define COLOR_BRIGHT_BLACK L"\033[90m"
#define COLOR_BRIGHT_RED L"\033[91m"
#define COLOR_BRIGHT_GREEN L"\033[92m"
#define COLOR_BRIGHT_YELLOW L"\033[93m"
#define COLOR_BRIGHT_BLUE L"\033[94m"
#define COLOR_BRIGHT_MAGENTA L"\033[95m"
#define COLOR_BRIGHT_CYAN L"\033[96m"
#define COLOR_BRIGHT_WHITE L"\033[97m"

#define BACKCOLOR_BLACK L"\033[40m"
#define BACKCOLOR_RED L"\033[41m"
#define BACKCOLOR_GREEN L"\033[42m"
#define BACKCOLOR_YELLOW L"\033[43m"
#define BACKCOLOR_BLUE L"\033[44m"
#define BACKCOLOR_MAGENTA L"\033[45m"
#define BACKCOLOR_CYAN L"\033[46m"
#define BACKCOLOR_WHITE L"\033[47m"
#define BACKCOLOR_BRIGHT_BLACK L"\033[100m"
#define BACKCOLOR_BRIGHT_RED L"\033[101m"
#define BACKCOLOR_BRIGHT_GREEN L"\033[102m"
#define BACKCOLOR_BRIGHT_YELLOW L"\033[103m"
#define BACKCOLOR_BRIGHT_BLUE L"\033[104m"
#define BACKCOLOR_BRIGHT_MAGENTA L"\033[105m"
#define BACKCOLOR_BRIGHT_CYAN L"\033[106m"
#define BACKCOLOR_BRIGHT_WHITE L"\033[107m"

#define COLOR_BYRGB(r, g, b) L"\033[38;2;" r L";" g L";" b L"m"
#define BACKCOLOR_BYRGB(r, g, b) L"\033[48;2;" r L";" g L";" b L"m"

#define POSITION_SAVE L"\033[s"
#define POSITION_RESTORE L"\033[u"

#define MOVE_UP(n) L"\033[" n L"A"
#define MOVE_DOWN(n) L"\033[" n L"B"
#define MOVE_RIGHT(n) L"\033[" n L"C"
#define MOVE_LEFT(n) L"\033[" n L"D"
#define NEXT_LINE(n) L"\e[1;1H\e[" n L"E"
#define PREV_LINE(n) L"\e[1;1H\e[" n L"F"
#define COLUMN_SET(n) L"\e[1;1H\e[" n L"G"
#define POSITION_SET(row, column) L"\033[" row L";" column L"H"

#define ALTERNATE_SCREEN_BUFFER_OPEN L"\033[?1049h"
#define ALTERNATE_SCREEN_BUFFER_CLOSE L"\033[?1049l"

#define TITLE_SET(title) L"\033]0;" title L"\007"
#define WTITLE_SET(title) L"\033]0;" title L"\007"

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_ANSI_H
