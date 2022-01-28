// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

// This header includes ANSI escape codes.

#ifndef __RANCH_ANSI
#define __RANCH_ANSI 1

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define CLEAR_SCREEN "\u001b[2J"
#define CLEAR_SCREEN_CB "\u001b[1J"
#define CLEAR_SCREEN_CE "\u001b[0J"

#define CLEAR_LINE "\u001b[2K"
#define CLEAR_LINE_CB "\u001b[1K"
#define CLEAR_LINECE "\u001b[0K"

#define SHOW_CURSOR "\033[?25h"
#define HIDE_CURSOR "\033[?25l"

#define FONT_RESET "\033[0m"
#define FONT_BOLD "\u001b[1m"
#define FONT_UNDERLINE "\u001b[4m"
#define FONT_REVERSED "\u001b[7m"

#define COLOR_BLACK "\033[30m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"
#define COLOR_BRIGHT_BLACK "\033[90m"
#define COLOR_BRIGHT_RED "\033[91m"
#define COLOR_BRIGHT_GREEN "\033[92m"
#define COLOR_BRIGHT_YELLOW "\033[93m"
#define COLOR_BRIGHT_BLUE "\033[94m"
#define COLOR_BRIGHT_MAGENTA "\033[95m"
#define COLOR_BRIGHT_CYAN "\033[96m"
#define COLOR_BRIGHT_WHITE "\033[97m"

#define BACKCOLOR_BLACK "\033[40m"
#define BACKCOLOR_RED "\033[41m"
#define BACKCOLOR_GREEN "\033[42m"
#define BACKCOLOR_YELLOW "\033[43m"
#define BACKCOLOR_BLUE "\033[44m"
#define BACKCOLOR_MAGENTA "\033[45m"
#define BACKCOLOR_CYAN "\033[46m"
#define BACKCOLOR_WHITE "\033[47m"
#define BACKCOLOR_BRIGHT_BLACK "\033[100m"
#define BACKCOLOR_BRIGHT_RED "\033[101m"
#define BACKCOLOR_BRIGHT_GREEN "\033[102m"
#define BACKCOLOR_BRIGHT_YELLOW "\033[103m"
#define BACKCOLOR_BRIGHT_BLUE "\033[104m"
#define BACKCOLOR_BRIGHT_MAGENTA "\033[105m"
#define BACKCOLOR_BRIGHT_CYAN "\033[106m"
#define BACKCOLOR_BRIGHT_WHITE "\033[107m"

#define COLOR_BYRGB(r, g, b) "\033[38;2;" r ";" g ";" b "m"
#define BACKCOLOR_BYRGB(r, g, b) "\033[48;2;" r ";" g ";" b "m"

#define POSITION_SAVE "\033[s"
#define POSITION_RESTORE "\033[u"

#define MOVE_UP(n) "\033[" n "A"
#define MOVE_DOWN(n) "\033[" n "B"
#define MOVE_RIGHT(n) "\033[" n "C"
#define MOVE_LEFT(n) "\033[" n "D"
#define NEXT_LINE(n) "\u001b[" n "E"
#define PREV_LINE(n) "\u001b[" n "F"
#define COLUMN_SET(n) "\u001b[" n "G"
#define POSITION_SET(row, column) "\033[" row ";" column "H"

#define ALTERNATE_SCREEN_BUFFER_OPEN "\033[?1049h"
#define ALTERNATE_SCREEN_BUFFER_CLOSE "\033[?1049l"

#define TITLE_SET(title) "\033]0;" title "\007"
#define WTITLE_SET(title) L"\033]0;" title L"\007"

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_ANSI
