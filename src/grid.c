#include "grid.h"
#include "config.h"
#include "utils.h"
#include <SDL_render.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef enum { EMPTY = 0, SAND } grid_val_t;

struct grid {
  int rows;
  int cols;
  grid_val_t *data;

  pos_t update_start;
  pos_t update_end;

  int tick_count;
};

/* private helper function to get a pointer
 * to the grid cell at a specific position.
 * Returns NULL when the position is outside
 * of the update range.
 */
grid_val_t *grid_value(grid_t *grid, pos_t pos) {
  if (pos.x < grid->update_start.x || pos.x >= grid->update_end.x ||
      pos.y < grid->update_start.y || pos.y >= grid->update_end.y) {
    return NULL;
  }

  return grid->data + grid->cols * pos.y + pos.x;
}

grid_t *grid_init(int rows, int cols) {
  grid_t *grid = malloc(sizeof(grid_t));
  assert(grid != NULL);

  grid->rows = rows;
  grid->cols = cols;

  grid->data = calloc(rows * cols, sizeof(grid_val_t));
  assert(grid->data != NULL);

  grid->tick_count = 0;

  pos_t size = {cols, rows};
  grid_resize(grid, size);

  srand(time(NULL));

  return grid;
}

void grid_destroy(grid_t *grid) {
  free(grid->data);
  free(grid);
}

void grid_tick(grid_t *grid) {

  for (int r = 0; r < grid->update_end.y - grid->update_start.y; r++) {
    for (int c = 0; c < grid->update_end.x - grid->update_start.x; c++) {
      int row = grid->update_end.y - 1 - r; // rows from top to bottom

      // col direction depends on the row for symmetry
      int col;
      if (row % 2 == 0) {
        col = grid->update_start.x + c;
      } else {
        col = grid->update_end.x - 1 - c;
      }
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

      if (center != NULL && *center == EMPTY) {
        // always prioritize an empty spot directly below
        *center = SAND;
        *val = EMPTY;
      } else if (left != NULL && *left == EMPTY) {
        *left = SAND;
        *val = EMPTY;
      } else if (right != NULL && *right == EMPTY) {
        *right = SAND;
        *val = EMPTY;
      }
    }
  }

  grid->tick_count++;
}

void draw_row_rect(SDL_Renderer *renderer, SDL_Rect rect, grid_val_t type) {
  if (type == SAND) {
    SDL_SetRenderDrawColor(renderer, SAND_COLOR_R, SAND_COLOR_G, SAND_COLOR_B,
                           255);
  } else if (type == EMPTY) {
    SDL_SetRenderDrawColor(renderer, EMPTY_COLOR_R, EMPTY_COLOR_G,
                           EMPTY_COLOR_B, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  }

  SDL_RenderFillRect(renderer, &rect);
}

void grid_draw(grid_t *grid, SDL_Renderer *renderer, pos_t window_size) {
  // rectangles are extended along a row as far as possible
  // before drawing
  bool rect_exists = false;
  grid_val_t rect_current_type;
  int rect_cell_count = 0;
  SDL_Rect rect;

  int num_cols = grid->update_end.x - grid->update_start.x;
  int num_rows = grid->update_end.y - grid->update_start.y;

  for (int r = 0; r < num_rows; r++) {
    rect_exists = false;

    for (int c = 0; c < num_cols; c++) {
      pos_t pos = {grid->update_start.x + c, grid->update_start.y + r};
      grid_val_t val = *grid_value(grid, pos);

      if (rect_exists && val != rect_current_type) {
        // draw rectangle if the current cell is a different type
        rect.w = window_size.x * rect_cell_count / num_cols + 2;
        rect.h = window_size.y / num_rows + 2;

        draw_row_rect(renderer, rect, rect_current_type);

        rect_exists = false;
      }

      if (!rect_exists) {
        // start new rect if there is not one that exists
        rect.x = c * window_size.x / num_cols - 1;
        rect.y = r * window_size.y / num_rows - 1;
        rect_cell_count = 0;
        rect_exists = true;
        rect_current_type = val;
      }
      rect_cell_count++;
    }

    // draw last rect in row
    rect.w = window_size.x * rect_cell_count / num_cols + 2;
    rect.h = window_size.y / num_rows + 2;
    draw_row_rect(renderer, rect, rect_current_type);
  }
}

void grid_resize(grid_t *grid, pos_t size) {
  int x_offset = (grid->cols - size.x) / 2;
  int y_offset = (grid->rows - size.y) / 2;

  pos_t new_start;
  new_start.x = x_offset;
  new_start.y = 2 * y_offset;

  pos_t new_end;
  new_end.x = grid->cols - x_offset;
  new_end.y = grid->rows;

  // clear left
  for (int col = grid->update_start.x; col < new_start.x; col++) {
    for (int row = grid->update_start.y; row < grid->update_end.y; row++) {
      pos_t pos = {col, row};
      *grid_value(grid, pos) = EMPTY;
    }
  }

  // clear right
  for (int col = new_end.x; col < grid->update_end.x; col++) {
    for (int row = grid->update_start.y; row < grid->update_end.y; row++) {
      pos_t pos = {col, row};
      *grid_value(grid, pos) = EMPTY;
    }
  }

  // clear top
  for (int col = grid->update_start.x; col < grid->update_end.x; col++) {
    for (int row = grid->update_start.y; row < new_start.y; row++) {
      pos_t pos = {col, row};
      *grid_value(grid, pos) = EMPTY;
    }
  }

  // set new update window
  grid->update_start = new_start;
  grid->update_end = new_end;
}

void grid_add_sand(grid_t *grid) {
  pos_t rel_pos = {rand() % (grid->update_end.x - grid->update_start.x - 8) + 4,
                   4};

  for (int r = -4; r <= 4; r++) {
    for (int c = -4; c <= 4; c++) {
      pos_t pos = {rel_pos.x + c + grid->update_start.x,
                   rel_pos.y + r + grid->update_start.y};

      grid_val_t *val = grid_value(grid, pos);
      if (val != NULL) {
        *val = SAND;
      }
    }
  }
}

void grid_remove_sand(grid_t *grid) {
  for (int row = grid->update_start.y; row < grid->update_end.y; row++) {
    for (int col = grid->update_start.x; col < grid->update_end.x; col++) {
      pos_t pos = {col, row};

      grid_val_t *val = grid_value(grid, pos);
      if (val != NULL && (rand() & 2) == 0) {
        *val = EMPTY;
      }
    }
  }
}
