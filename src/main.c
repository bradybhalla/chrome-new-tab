#include "program.h"
#include <SDL2/SDL.h>
#include <emscripten.h>

void main_loop(void *data) {
  program_t *program = (program_t *)(data);

  bool running = program_update(program, SDL_GetTicks64());
  if (!running) {
    program_destroy(program);
  }
}

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO);

  program_t *program = program_init();

  emscripten_set_main_loop_arg(main_loop, program, 0, true);

  return 0;
}
