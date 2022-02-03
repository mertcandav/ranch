// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_LIST_H
#define __RANCH_LIST_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Dynamic list.
typedef struct list {
  void   **array;
  size_t used;
  size_t size;
} list;

// Create new list instance allocated from heap by specified size.
struct list *list_new(size_t size);
// Free list instance allocated from heap.
void list_free(struct list *lst);
// Appends item to list.
void list_push(struct list *lst, void *item);
// Removes n elements starts at specified index.
// If n greater than size, uses size instead of n.
//
// Special cases are:
//  list_remrange(lst, start, n) = nothing if n < 1
//  list_remrange(lst, start, n) = nothing if start < 0
//  list_remrange(lst, start, n) = nothing if start > size
void list_remrange(struct list* lst, const size_t start, size_t n);
// Returns new list from source list by specified index and n.
// If n greater than size, uses size instead of n.
//
// Special cases are:
//  list_slice(lst, start, n) = NULL if lst == NULL
//  list_slice(lst, start, n) = NULL if n < 1
//  list_slice(lst, start, n) = NULL if start < 0
//  list_slice(lst, start, n) = NULL if start > size
struct list *list_slice(struct list *lst, size_t start, size_t n);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_LIST_H
