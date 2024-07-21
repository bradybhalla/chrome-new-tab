#include <SDL2/SDL.h>
#include <emscripten.h>
#include <iostream>
#include "grid.hpp"

SDL_Rect make_rect(int x, int y, int w, int h) {
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  return rect;
}

struct Program {
  Program() {
    SDL_CreateWindowAndRenderer(500, 500, SDL_WINDOW_RESIZABLE, &window,
                                &renderer);
  }
  void update(uint64_t time) {
    SDL_Event e;
    SDL_PollEvent(&e);
    switch (e.type) {
    case SDL_QUIT:
      SDL_DestroyWindow(window);
      SDL_DestroyRenderer(renderer);
      return;
    case SDL_KEYDOWN:
      std::cout << "keydown" << std::endl;
      break;
    case SDL_KEYUP:
      std::cout << "keyup" << std::endl;
      break;
    case SDL_WINDOWEVENT:
      std::cout << "window event" << std::endl;
      break;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect rect =
        make_rect(250, 150, static_cast<int>(time) % 1000 / 5.0, 200);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
  }

private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  Grid sand_grid;
};

void main_loop(void *data) {
  Program &prog = *static_cast<Program *>(data);
  prog.update(SDL_GetTicks64());
}

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO);

  Program arg_data = Program();

  emscripten_set_main_loop_arg(main_loop, &arg_data, 0, true);

  return 0;
}
