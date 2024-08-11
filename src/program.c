#include "program.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <assert.h>

struct program {
  SDL_Window *window;
  SDL_Renderer *renderer;

  pos_t window_size;
  uint64_t cur_time;
};

// helper function to resize the program if needed
void program_update_resized(program_t *program) {
  pos_t new_size;
  SDL_GetWindowSize(program->window, &new_size.x, &new_size.y);

  if (program->window_size.x != new_size.x ||
      program->window_size.y != new_size.y) {
    program->window_size = new_size;
    printf("window is resized\n");
  }
}

// helper function to display the program
void program_display(program_t *program) {
  SDL_SetRenderDrawColor(program->renderer, 0, 0, 0, 255);
  SDL_RenderClear(program->renderer);

  SDL_Rect rect;
  rect.x = 250;
  rect.y = 150;
  rect.w = program->cur_time % 1000 / 5.0;
  rect.h = 200;

  SDL_SetRenderDrawColor(program->renderer, 10, 100, 100, 255);
  SDL_RenderFillRect(program->renderer, &rect);

  SDL_RenderPresent(program->renderer);
}

program_t *program_init() {
  program_t *program = malloc(sizeof(program_t));
  assert(program != NULL);

  SDL_CreateWindowAndRenderer(500, 500, SDL_WINDOW_RESIZABLE, &program->window,
                              &program->renderer);

  // set to an impossible size so the values will
  // change on the first update
  program->window_size.x = -1;
  program->window_size.y = -1;

  program->cur_time = 0;

  printf("program initialized\n");

  return program;
}

void program_destroy(program_t *program) {
  SDL_DestroyWindow(program->window);
  SDL_DestroyRenderer(program->renderer);
  free(program);
  printf("program destroyed\n");
}

bool program_update(program_t *program, uint64_t time) {
  uint64_t dt = time - program->cur_time;
  program->cur_time = time;

  SDL_Event e;
  SDL_PollEvent(&e);
  switch (e.type) {
  case SDL_QUIT:
    program_destroy(program);
    return false;
  case SDL_KEYDOWN:
    printf("keydown\n");
    break;
  case SDL_KEYUP:
    printf("keyup\n");
    break;
  case SDL_WINDOWEVENT:
    program_update_resized(program);
    break;
  }

  program_display(program);

  return true;
}
