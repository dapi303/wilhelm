#include "fps.h"
#include "const.h"

#include <SDL_timer.h>

void Fps::init() {
  lastUpdate = SDL_GetTicks();
  fps = 0.0f;
}

void Fps::frameStart() {
  Uint32 currentUpdate = SDL_GetTicks();
  currentFrameStart = SDL_GetPerformanceCounter();

  if (lastUpdate > 0) {
    deltaTime = (currentUpdate - lastUpdate) / 1000.0f;
  }
  lastUpdate = currentUpdate;

  if (deltaTime > 0 &&
      currentUpdate - lastCounterUpdate > float(CONST::fpsUpdateDelaysMs)) {
    lastCounterUpdate = currentUpdate;
    fps = 1000.0f / deltaTime / 1000.0f;
  }
}

void Fps::frameEnd() {
  Uint64 currentFrameEnd = SDL_GetPerformanceCounter();
  float elapsedMS = (currentFrameEnd - currentFrameStart) /
                    (float)SDL_GetPerformanceFrequency() * 1000.0f;

  if (elapsedMS < 16.666f) {
    SDL_Delay(floor(16.666f - elapsedMS));
  }
}

float const Fps::getDeltaTime() { return deltaTime; }

float const Fps::getFps() { return fps; }
