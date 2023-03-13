#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <GL/glew.h>
#include "hud.h"
#include "ui/button.h"

Hud::Hud(Fps &fps) : fps(fps) {}

void Hud::init(const int width, const int height) {
  this->width = width;
  this->height = height;

  text.init(width, height);
  glUseProgram(uiShader);
  // glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f,
  // static_cast<float>(height));
  glm::mat4 scale =
      glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / width, 1.0f / height, 1.0f));
  glUniformMatrix4fv(glGetUniformLocation(uiShader, "scale"), 1, GL_FALSE,
                     glm::value_ptr(scale));
  buttons.push_back(Button(uiShader, text, "Ibcde", width / 2, height / 2));
  // Button(uiShader, text, "button w/2 h/2", width / 2, height / 2));
  // buttons.push_back(Button(uiShader, text, "12", 200, 400));
  // buttons.push_back(Button(uiShader, text, "123", 100, 200));
  // buttons.push_back(Button(uiShader, text, "abcde", 0, 200));
  // buttons.push_back(Button(uiShader, text, "button 200 0", 200, 0));
  // buttons.push_back(Button(uiShader, text, "but", 200, 180));
}

void const Hud::render() {
  // glEnable(GL_CULL_FACE);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  std::vector<std::string> lines = {
      "FPS:" + std::to_string((int)fps.getFps()),
  };
  text.renderBelow(lines, 0, height, 0.5f);

  for (auto button : buttons) {
    button.render();
  }
  // renderButton("labelX", text, 0, 400);

  // glDisable(GL_CULL_FACE);
  // glDisable(GL_BLEND);
}
