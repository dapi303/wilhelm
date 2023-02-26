#include "hud.h"

HUD::HUD(Fps &fps, AppWindow &appWindow) : appWindow(appWindow), fps(fps) {}

int HUD::init() {
  text.init(appWindow);
  return 0;
}

void HUD::render() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  text.renderTop(std::to_string((int)fps.getFps()) + " fps", 0.0f,
                 appWindow.height, 0.3f);

  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}
