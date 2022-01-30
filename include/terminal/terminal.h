// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_TERMINAL
#define __RANCH_TERMINAL 1

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef __WIN32
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif // __WIN32

#ifdef __WIN32
// Enables Virtual Terminal Processing on Windows.
void enable_virtual_terminal_processing(void);
#endif // __WIN32

typedef struct terminal {
  unsigned char inloop;
  // Prints before reads each command-line input.
  wchar_t       *routine_message;
  // Seperator for routine message.
  // Prints after printed the routine message.
  wchar_t       *sep;
} terminal;

// Create new terminal instance allocated from heap.
struct terminal *terminal_new(void);
// Free terminal instance allocated from heap.
void terminal_free(struct terminal *term);
// Returns once input from command-line with terminal configs.
// Return value is heap-allocated.
wchar_t *once(struct terminal *term);
// Into the command-line input loop with specified function.
// The parameter of specified function, takes input from command-line
// with self "once" method.
void terminal_loop(struct terminal *term, void(*f)(wchar_t *input));
// Stops loop if looping.
void terminal_stop(struct terminal *term);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_TERMINAL
