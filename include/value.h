// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_VALUE_H
#define __RANCH_VALUE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef double ranch_number;

// Computed value piece.
typedef struct value {
  ranch_number data;
} value;

// Create new value instance allocated from heap.
struct value *value_new(void);
// Free value instance allocated from heap.
void value_free(struct value *val);
void value_print(const struct value *val);
// Frees destination value and set instead src.
void value_repl(struct value *dest, struct value *src);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_VALUE_H
