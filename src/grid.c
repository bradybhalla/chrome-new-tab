#include "grid.h"
#include "config.h"
#include "utils.h"
#include <assert.h>
#include <stdlib.h>

typedef enum { EMPTY = 0, SAND, BLOCKED } grid_val_t;

struct grid {
  size_t rows;
  size_t cols;
  grid_val_t *data;

  pos_t update_start;
  pos_t update_end;
};

/* private helper function to get a pointer
 * to the grid cell at a specific position
 */
grid_val_t *grid_value(grid_t *grid, pos_t pos) {
  return grid->data + grid->cols * pos.y + pos.x;
}

grid_t *grid_init(size_t rows, size_t cols) {
  grid_t *grid = malloc(sizeof(grid_t));
  assert(grid != NULL);

  grid->rows = rows;
  grid->cols = cols;

  grid->data = calloc(rows * cols, sizeof(grid_val_t));
  assert(grid->data != NULL);

  for (size_t i = 0; i < cols; i++) {
    pos_t pos = {i, rows - 1};
    *grid_value(grid, pos) = BLOCKED;
  }
  for (size_t i = 0; i < rows; i++) {
    pos_t pos = {0, i};
    *grid_value(grid, pos) = BLOCKED;
  }
  for (size_t i = 0; i < rows; i++) {
    pos_t pos = {cols - 1, i};
    *grid_value(grid, pos) = BLOCKED;
  }

  pos_t size = {cols, rows};
  grid_resize(grid, size);

  return grid;
}

void grid_destroy(grid_t *grid) {
  free(grid->data);
  free(grid);
}

void grid_update(grid_t *grid, uint64_t time) {

  for (int row = grid->update_end.y + 1; row >= grid->update_start.y; row--) {
    for (int col = grid->update_start.x; col < grid->update_end.x; col++) {
      pos_t pos = {col, row};

      grid_val_t *val = grid_value(grid, pos);
      if (*val != SAND) {
        continue;
      }

      pos.y = row + 1;
      pos.x = col - 1;
      grid_val_t *left = grid_value(grid, pos);
      pos.x = col;
      grid_val_t *center = grid_value(grid, pos);
      pos.x = col + 1;
      grid_val_t *right = grid_value(grid, pos);

      if (*center == EMPTY) {
        *center = SAND;
        *val = EMPTY;
      } else if (*left == EMPTY) {
        *left = SAND;
        *val = EMPTY;
      } else if (*right == EMPTY) {
        *right = SAND;
        *val = EMPTY;
      }
    }
  }

  pos_t pos = {grid->update_start.x +
                   time / 1000 * 51 %
                       (grid->update_end.x - grid->update_start.x - 10) +
                   5,
               grid->update_start.y + 1};
  grid_val_t *val = grid_value(grid, pos);
  *val = SAND;
}

void grid_draw(grid_t *grid, SDL_Renderer *renderer, pos_t window_size) {
  int num_cols = grid->update_end.x - grid->update_start.x;
  int num_rows = grid->update_end.y - grid->update_start.y;

  for (size_t r = 0; r < num_rows; r++) {
    for (size_t c = 0; c < num_cols; c++) {
      pos_t pos = {grid->update_start.x + c, grid->update_start.y + r};

      grid_val_t val = *grid_value(grid, pos);

      if (val == SAND) {
        SDL_SetRenderDrawColor(renderer, SAND_COLOR_R, SAND_COLOR_G,
                               SAND_COLOR_B, 255);
      } else if (val == EMPTY) {
        SDL_SetRenderDrawColor(renderer, EMPTY_COLOR_R, EMPTY_COLOR_G,
                               EMPTY_COLOR_B, 255);
      } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      }

      SDL_Rect rect;
      rect.x = c * window_size.x / num_cols - 1;
      rect.y = r * window_size.y / num_rows - 1;
      rect.w = window_size.x / num_cols + 2;
      rect.h = window_size.y / num_rows + 2;

      SDL_RenderFillRect(renderer, &rect);
    }
  }
}

void grid_resize(grid_t *grid, pos_t size) {
  size_t x_offset = (grid->cols - size.x) / 2;
  size_t y_offset = (grid->rows - size.y) / 2;

  pos_t new_start;
  new_start.x = max(x_offset, 1);
  new_start.y = max(2 * y_offset, 0);

  pos_t new_end;
  new_end.x = min(grid->cols - x_offset, grid->cols - 1);
  new_end.y = grid->rows - 1;

  size_t min_row = min(new_start.y, grid->update_start.y);
  size_t max_row = max(new_end.y, grid->update_end.y);

  size_t min_col = min(new_start.x, grid->update_start.x);
  size_t max_col = max(new_end.x, grid->update_end.x);

  // clear left
  for (size_t col = max(grid->update_start.x - 1, 1); col < new_start.x;
       col++) {
    for (size_t row = min_row; row < max_row; row++) {
      pos_t pos = {col, row};
      *grid_value(grid, pos) = EMPTY;
    }
  }

  // clear right
  for (size_t col = new_end.x;
       col < min(grid->update_end.x + 1, grid->cols - 1); col++) {
    for (size_t row = min_row; row < max_row; row++) {
      pos_t pos = {col, row};
      *grid_value(grid, pos) = EMPTY;
    }
  }

  // clear top
  for (size_t col = min_col; col < max_col; col++) {
    for (size_t row = grid->update_start.y; row < new_start.y; row++) {
      pos_t pos = {col, row};
      *grid_value(grid, pos) = EMPTY;
    }
  }

  // set new update window
  grid->update_start = new_start;
  grid->update_end = new_end;
}
