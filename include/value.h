// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_VALUE
#define __RANCH_VALUE 1

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define TRUE 1
#define FALSE 0

typedef unsigned char ranch_bool;
typedef double ranch_number;

// Computed value piece.
typedef struct value {
  ranch_number data;
} value;

// Create new value instance allocated from heap.
value *value_new(void);
// Free value instance allocated from heap.
void value_free(value *val);
void value_print(const value *val);
// Frees destination value and set instead src.
void value_repl(value *dest, value *src);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_VALUE
