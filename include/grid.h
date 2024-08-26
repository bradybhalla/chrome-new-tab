#ifndef GRID_H
#define GRID_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include "utils.h"

typedef struct grid grid_t;

/* Create a new grid and return a pointer */
grid_t *grid_init(int rows, int cols);

/* Destroy grid and free all allocated space. */
void grid_destroy(grid_t* grid);

/* Updates the grid */
void grid_tick(grid_t* grid);

/* Change number of cells which are displayed and updated */
void grid_resize(grid_t *grid, pos_t size);

/* Draw the grid */
void grid_draw(grid_t* grid, SDL_Renderer* renderer, pos_t window_size);

/* Add sand randomly */
void grid_add_sand(grid_t *grid);

/* Remove sand randomly */
void grid_remove_sand(grid_t *grid);

#endif
