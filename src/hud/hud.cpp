#include "hud.h"

HUD::HUD(Fps &fps) : fps(fps) {}

int HUD::init(const AppWindow &appWindow) {
  this->text.init(appWindow);
  return 0;
}

void HUD::render() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  text.render("This is sample text", 25.0f, 25.0f, 1.0f,
              glm::vec3(0.5, 0.8f, 0.2f));

  text.render(std::to_string((int)fps.getFps()) + " fps", 0.0f, 400.0f, 1.0f,
              glm::vec3(0.5, 0.7f, 0.2f));

  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}
