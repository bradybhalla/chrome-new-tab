#include "config.h"
#include "grid.h"
#include <SDL2/SDL.h>
#include <assert.h>
#include <emscripten.h>
#include <stdbool.h>

typedef struct program {
  SDL_Window *window;
  SDL_Renderer *renderer;

  pos_t window_size;
  uint64_t last_grid_update_time;

  grid_t *grid;
} program_t;

bool program_size_changed(program_t *program) {
  pos_t new_size;
  SDL_GetWindowSize(program->window, &new_size.x, &new_size.y);

  return program->window_size.x != new_size.x ||
         program->window_size.y != new_size.y;
}

void program_resized(program_t *program) {
  SDL_GetWindowSize(program->window, &program->window_size.x,
                    &program->window_size.y);

  if (program->grid != NULL) {
    int cell_size_x = program->window_size.x / GRID_COLS + 1;
    int cell_size_y = program->window_size.y / GRID_ROWS + 1;
    int cell_size = max(max(cell_size_x, cell_size_y), MIN_CELL_SIZE);

    pos_t new_grid_size = {program->window_size.x / cell_size,
                           program->window_size.y / cell_size};

    grid_resize(program->grid, new_grid_size);
  }
}

program_t *program_init() {
  program_t *program = malloc(sizeof(program_t));
  assert(program != NULL);

  SDL_CreateWindowAndRenderer(500, 500, SDL_WINDOW_RESIZABLE, &program->window,
                              &program->renderer);

  program->last_grid_update_time = 0;

  program->grid = NULL;

  program_resized(program);

  return program;
}

void program_destroy(program_t *program) {
  SDL_DestroyWindow(program->window);
  SDL_DestroyRenderer(program->renderer);
  if (program->grid != NULL) {
    grid_destroy(program->grid);
  }
  free(program);
}

void program_initialize_grid(program_t *program, uint64_t time) {
  if (program->grid == NULL) {
    program->grid = grid_init(GRID_ROWS, GRID_COLS);
    program->last_grid_update_time = time;
    program_resized(program);
  }
}

void loop(void *data) {
  program_t *program = (program_t *)(data);

  uint64_t time = SDL_GetTicks64();

  // handle events
  SDL_Event e;
  while (SDL_PollEvent(&e) == 1) {
    switch (e.type) {
    case SDL_QUIT: {
      return;
    }
    case SDL_KEYDOWN: {
      program_initialize_grid(program, time);
      if (e.key.keysym.sym != SDLK_BACKSPACE) {
        grid_add_sand(program->grid);
      } else {
        grid_remove_sand(program->grid);
      }
      break;
    }
    case SDL_WINDOWEVENT: {
      if (program_size_changed(program)) {
        program_resized(program);
      }
      break;
    }
    }
  }

  SDL_SetRenderDrawColor(program->renderer, EMPTY_COLOR_R, EMPTY_COLOR_G,
                         EMPTY_COLOR_B, 255);
  SDL_RenderClear(program->renderer);

  if (program->grid != NULL) {

    // update grid
    while (program->last_grid_update_time < time) {
      grid_tick(program->grid);
      program->last_grid_update_time += GRID_UPDATE_MS;
    }

    // draw grid
    grid_draw(program->grid, program->renderer, program->window_size);
  }

  SDL_RenderPresent(program->renderer);
}

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO);

  program_t *program = program_init();

  emscripten_set_main_loop_arg(loop, program, 0, true);

  program_destroy(program);

  return 0;
}
