#include <SDL2/SDL.h>
#include <stdio.h>

//#include "shaders/shader.h"
#include "app.h"

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 1200

int main(int argc, char *argv[]) {
  SDL_Log("init");
  App app;
  int code = app.init(SCREEN_WIDTH, SCREEN_HEIGHT);

  if (0 == code) {
    app.loop();
  } else {
    SDL_Log("failed to init, code:%d\n", code);
  }

  SDL_Quit();

  return 0;
}
