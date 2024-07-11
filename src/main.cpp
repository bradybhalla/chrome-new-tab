#include <iostream>
#include <SDL2/SDL.h>
#include <emscripten.h>

void loop(){

}

int main() {
  for (size_t i = 0; i < 100; i++) {
    std::cout << "hello there: " << i << std::endl;
  }
  return 0;
}
