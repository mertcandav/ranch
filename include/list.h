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
// Removed elements between this range, except "end" index.
void list_remrange(struct list* lst, const size_t start, size_t end);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_LIST_H
