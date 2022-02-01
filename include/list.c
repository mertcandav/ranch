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
