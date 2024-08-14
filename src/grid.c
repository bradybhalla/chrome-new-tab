#include "grid.h"
#include "utils.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

struct grid {
  size_t rows;
  size_t cols;
  bool *data;
};

/*
 * Helper function to get the data index
 * from a grid position
 */
size_t grid_index_of_pos(grid_t *grid, pos_t pos) {
  return grid->cols * pos.y + pos.x;
}

grid_t *grid_init(size_t rows, size_t cols) {
  grid_t *grid = malloc(sizeof(grid_t));
  assert(grid != NULL);

  grid->rows = rows;
  grid->cols = cols;

  grid->data = calloc(rows * cols, sizeof(bool));
  assert(grid->data != NULL);

  for (size_t i = 0; i < rows * cols; i += 3) {
    grid->data[i] = 1;
  }

  return grid;
}

void grid_destroy(grid_t *grid) {
  free(grid->data);
  free(grid);
}

void grid_update(grid_t *grid) {}

void grid_draw(grid_t *grid, SDL_Renderer *renderer, pos_t window_size) {
  for (size_t col = 0; col < grid->rows; col++) {
    for (size_t row = 0; row < grid->cols; row++) {
      pos_t pos = {col, row};

      if (grid->data[grid_index_of_pos(grid, pos)]) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
      } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      }

      SDL_FRect rect;
      rect.x = (float)col * window_size.x / grid->cols;
      rect.y = (float)row * window_size.y / grid->rows;
      rect.w = (float)window_size.x / grid->cols;
      rect.h = (float)window_size.y / grid->rows;

      SDL_RenderFillRectF(renderer, &rect);
    }
  }
}
