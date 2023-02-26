#include "hud.h"

HUD::HUD(Fps &fps, AppWindow &appWindow, Creature &player)
    : appWindow(appWindow), fps(fps), player(player) {}

int HUD::init() {
  text.init(appWindow);
  return 0;
}

void HUD::render() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  renderDebug();

  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}

void HUD::renderDebug() {
  std::vector<std::string> lines = {
      "FPS:" + std::to_string((int)fps.getFps()),
      "",
      "player:",
      "X: " + std::to_string(int(player.x * 100)),
      "Y: " + std::to_string(int(player.y * 100)),
      "Z: " + std::to_string(int(player.z * 100)),
      "",
      "target",
      "active: " + std::to_string(player.target.active),
      "X: " + std::to_string(int(player.target.x * 100)),
      "Y: " + std::to_string(int(player.target.y * 100)),
      "Z: " + std::to_string(int(player.target.z * 100)),
  };

  text.renderTop(lines, 0.0f, appWindow.height, 0.7f);
}
