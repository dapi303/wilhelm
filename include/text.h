#ifndef text__h
#define text__h

#include "./font.h"
#include "./shaders.h"
//#include "../structs.h"

#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <vector>

class Text {
public:
  int init(const int width, const int height);
  void const render(std::string text, float x, float y, float scale,
                    glm::vec3 color = glm::vec3(0.5f, 0.8f, 0.2f));

  void const render(std::vector<std::string> text, float x, float y,
                    float scale, glm::vec3 color = glm::vec3(0.5f, 0.8f, 0.2f));

  // render text below given position
  void const renderBelow(std::string text, float x, float y, float scale,
                         glm::vec3 color = glm::vec3(0.5f, 0.8f, 0.2f));
  void const renderBelow(std::vector<std::string> lines, float x, float y,
                         float scale,
                         glm::vec3 color = glm::vec3(0.5f, 0.8f, 0.2f));

private:
  Characters fontChars;
  unsigned int lineHeight;
  unsigned int VAO, VBO;
};
#endif
