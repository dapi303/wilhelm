#ifndef text__h
#define text__h

#include "font.h"
#include "shaders/shaders.h"
#include "structs.h"

#include "glm/gtc/matrix_transform.hpp"

class Text {
public:
  int init(const AppWindow &appWindow);
  void render(std::string text, float x, float y, float scale, glm::vec3 color);

private:
  Characters fontChars;
  Shader *shader;
  unsigned int VAO, VBO;
};
#endif
