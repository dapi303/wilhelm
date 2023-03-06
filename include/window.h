#ifndef window__h
#define window__h

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window {
public:
  Window(int width, int height);
  bool create();
  GLFWwindow *getInstance() const;
  void close();
  glm::mat4 getProjection() const;

private:
  int width;
  int height;
  GLFWwindow *window;
  glm::mat4 projection;
};

#endif
