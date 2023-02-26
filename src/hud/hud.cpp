#include "hud.h"

#include <sstream>
#include <iomanip>

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

std::string getStr(std::stringstream &ss, float f) {
  ss.str("");
  ss << std::fixed << std::setprecision(3) << f;
  return ss.str();
}

void HUD::renderDebug() {
  std::stringstream stream;

  std::vector<std::string> lines = {
      "FPS:" + std::to_string((int)fps.getFps()),
      "",
      "player:",
      "X: " + getStr(stream, player.x),
      "Y: " + getStr(stream, player.y),
      "Z: " + getStr(stream, player.z),
      "",
      "target",
      "active: " + std::to_string(player.target.active),
      "X: " + getStr(stream, player.target.x),
      "Y: " + getStr(stream, player.target.y),
      "Z: " + getStr(stream, player.target.z),
  };

  text.renderTop(lines, 0.0f, appWindow.height, 0.7f);
}
