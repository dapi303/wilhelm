#ifndef mouse__h
#define mouse__h

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>

class Mouse {
public:
  Mouse(GLFWwindow *window, glm::vec3 cameraPosition,
        glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
  void init(GLFWwindow *window, glm::vec3 cameraPosition,
            glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

  glm::vec3 const getMouse3DPosition();

private:
  glm::vec3 const translateToWorldCords(glm::vec2 position);

  void static clickCallback(GLFWwindow *window, int button, int action,
                            int mods);
  void static moveCallback(GLFWwindow *window, double xpos, double ypos);

  glm::vec3 cameraPosition;
  glm::mat4 projectionMatrix;
  glm::mat4 viewMatrix;

  glm::vec2 mousePositionRawPx;
};

#endif
