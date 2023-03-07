#include "hud.h"

void Hud::init(const int width, const int height) {
  this->width = width;
  this->height = height;

  text.init(width, height);
}

void const Hud::render() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  text.renderBelow("fps", 0, height, 1.0f);

  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}
