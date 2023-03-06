#include "fps.h"
#include <GLFW/glfw3.h>
#include <cstdio>

#define MAX_FPS 100
#define FPS_UPDATE_DELAY_S 0.3f

void Fps::init() {
  lastUpdateS = glfwGetTime();
  fps = 0.0f;
  minFrameTimeMs = 1000.0f / MAX_FPS;
}

void Fps::update() {
  double currentUpdateS = glfwGetTime();

  if (lastUpdateS > 0) {
    deltaTimeS = (currentUpdateS - lastUpdateS);
  }
  lastUpdateS = currentUpdateS;

  if (deltaTimeS > 0 &&
      currentUpdateS - lastCounterUpdateS > FPS_UPDATE_DELAY_S) {
    lastCounterUpdateS = currentUpdateS;
    fps = 1000.0f / deltaTimeS / 1000.0f;
  }
}

float const Fps::getDeltaTime() { return deltaTimeS; }

float const Fps::getFps() { return fps; }
