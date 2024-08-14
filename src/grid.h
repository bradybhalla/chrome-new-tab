#ifndef GRID_H
#define GRID_H

#include "utils.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct grid grid_t;

/* Create a new grid and return a pointer */
grid_t *grid_init(size_t rows, size_t cols);

/* Destroy grid and free all allocated space. */
void grid_destroy(grid_t* grid);

/* Updates the grid */
void grid_update(grid_t* grid);

/* Draw the grid */
void grid_draw(grid_t* grid, SDL_Renderer* renderer, pos_t window_size);

#endif
