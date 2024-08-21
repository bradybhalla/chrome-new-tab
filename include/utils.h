#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

typedef struct {
  int x;
  int y;
} pos_t;

size_t max(size_t x, size_t y);
size_t min(size_t x, size_t y);

#endif
