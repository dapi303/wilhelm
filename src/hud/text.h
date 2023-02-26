#ifndef text__h
#define text__h

#include "../font.h"
#include "../shaders/shaders.h"
#include "../structs.h"

#include "glm/gtc/matrix_transform.hpp"

class Text {
public:
  int init(const AppWindow &appWindow);
  void render(std::string text, float x, float y, float scale,
              glm::vec3 color = glm::vec3(0.5f, 0.8f, 0.2f));

  void render(std::vector<std::string> text, float x, float y, float scale,
              glm::vec3 color = glm::vec3(0.5f, 0.8f, 0.2f));

  // render text below given position
  void renderTop(std::string text, float x, float y, float scale,
                 glm::vec3 color = glm::vec3(0.5f, 0.8f, 0.2f));
  void renderTop(std::vector<std::string> lines, float x, float y, float scale,
                 glm::vec3 color = glm::vec3(0.5f, 0.8f, 0.2f));

private:
  Characters fontChars;
  unsigned int lineHeight;
  Shader *shader;
  unsigned int VAO, VBO;
};
#endif
