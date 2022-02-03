#include <stdio.h>
#include <wchar.h>

#include "list.h"
#include "messages.h"

struct list *list_new(size_t size) {
  struct list *lst = (struct list*)(malloc(sizeof(struct list)));
  if (!lst) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  if (size == 0) { ++size; }
  lst->array = (void**)(malloc(size*sizeof(void*)));
  if (!lst->array) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  lst->used = 0;
  lst->size = size;
  return lst;
}

void list_free(struct list *lst) {
  if (!lst) { return; }
  free(lst->array);
  lst->array = NULL;
  lst->used = 0;
  lst->size = 0;
  free(lst);
  lst = NULL;
}

void list_push(struct list* lst, void *item) {
  if (lst->size <= lst->used) {
    lst->size *= 2;
    lst->array = realloc(lst->array, lst->size*sizeof(void*));
    if (!lst->array) {
      wprintf(ERROR_ALLOCATION_FAILED L"\n");
      exit(EXIT_FAILURE);
    }
  }
  lst->array[lst->used++] = item;
}

void list_remrange(struct list *lst, const size_t start, size_t n) {
       if (start < 0)         { return; }
  else if (start > lst->used) { return; }
  else if (n < 1)             { return; }
  if (n > lst->used-start) { n = lst->used; }
  struct list *new_lst = list_new(lst->used-n);
  for (size_t index = 0; index < start; ++index)
  { list_push(new_lst, lst->array[index]); }
  for (size_t index = start+n; index < lst->used; ++index)
  { list_push(new_lst, lst->array[index]); }
  lst->used = new_lst->used;
  lst->size = new_lst->size;
  free(lst->array);
  lst->array = NULL;
  *lst = *new_lst;
  new_lst->size = 0;
  new_lst->used = 0;
  new_lst->array = NULL;
  free(new_lst);
  new_lst = NULL;
}

struct list *list_slice(struct list *lst, size_t start, size_t n) {
       if (!lst)      { return NULL; }
  else if (start < 0) { return NULL; }
  else if (n < 1)     { return NULL; }
  if (n > lst->used-start) { n = lst->used; }
  struct list* slice = list_new(n);
  for (; n >= 0; --n) { list_push(slice, lst->array[start++]); }
  return slice;
}
