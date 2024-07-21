#include "grid.hpp"
#include "utils.hpp"
#include <SDL2/SDL.h>
#include <SDL_video.h>
#include <emscripten.h>
#include <iostream>
#include <utility>

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

    SDL_GetWindowSize(window, &window_size.first, &window_size.second);
    cur_time = 0;
    update_resized();
    display();
  }

  void update(uint64_t time) {
    uint64_t dt = time - cur_time;
    cur_time = time;

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
      pos new_size;
      SDL_GetWindowSize(window, &new_size.first, &new_size.second);

      if (window_size != new_size) {
        window_size = new_size;
        update_resized();
      }
      break;
    }

    display();
  }

  void update_resized() { std::cout << "window is resized" << std::endl; }

  void display() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect rect =
        make_rect(250, 150, static_cast<int>(cur_time) % 1000 / 5.0, 200);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
  }

private:
  SDL_Window *window;
  SDL_Renderer *renderer;

  pos window_size;
  uint64_t cur_time;

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
