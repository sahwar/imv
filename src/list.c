#include "list.h"

struct list *list_create(void)
{
  struct list *list = malloc(sizeof(struct list));
  list->len = 0;
  list->cap = 64;
  list->items = malloc(sizeof(void*) * list->cap);
  return list;
}

void list_free(struct list *list)
{
  free(list->items);
}

void list_deep_free(struct list *list)
{
  for(size_t i = 0; i < list->len; ++i) {
    free(list->items[i]);
  }
  list_free(list);
}

void list_append(struct list *list, void *item)
{
  list_grow(list, list->len + 1);
  list->items[list->len++] = item;
}

void list_grow(struct list *list, size_t min_size)
{
  if(list->cap >= min_size) {
    return;
  }

  while(list->cap < min_size) {
    list->cap *= 2;
  }

  list->items = realloc(list->items, sizeof(void*) * list->cap);
}

void list_remove(struct list *list, size_t index)
{
  if(index >= list->len) {
    return;
  }

  memmove(&list->items[index], &list->items[index + 1], list->len - index);

  list->len -= 1;
}

void list_insert(struct list *list, size_t index, void *item)
{
  list_grow(list, list->len + 1);

  if(index > list->len) {
    index = list->len;
  }

  memmove(&list->items[index + 1], &list->items[index], list->len - index);
  list->items[index] = item;
  list->len += 1;
}

struct list *list_from_string(const char *string, char delim)
{
  struct list *list = list_create();

  const char *base = string;

  while(*base) {
    while(*base && *base == delim) {
      ++base;
    }

    const char *end = base;
    while(*end && *end != delim) {
      ++end;
    }

    if(*base && base != end) {
      char *item = strndup(base, end - base);
      list_append(list, item);
      base = end;
    }
  }

  return list;
}

int list_find(struct list *list, int (*cmp)(const void *, const void *), const void *key)
{
  for(size_t i = 0; i < list->len; ++i) {
    if(!cmp(list->items[i], key)) {
      return (int)i;
    }
  }
  return -1;
}

/* vim:set ts=2 sts=2 sw=2 et: */
