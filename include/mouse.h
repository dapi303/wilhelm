#ifndef mouse__h
#define mouse__h

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>
#include <optional>
#include <queue>

enum EventType { CLICK = 0, SCROLL };
struct MouseEvent {
  EventType type;
  double x;
  double y;
};

class Mouse {
public:
  Mouse(GLFWwindow *window);
  void init(GLFWwindow *window, glm::vec3 cameraPosition,
            glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

  glm::vec3 const getMouse3DPosition();
  bool clicked = false;

  std::optional<MouseEvent> popEvent();

private:
  glm::vec3 const translateToWorldCords(glm::vec2 position);

  void static clickCallback(GLFWwindow *window, int button, int action,
                            int mods);
  void static moveCallback(GLFWwindow *window, double xpos, double ypos);
  void static scrollCallback(GLFWwindow *window, double xoffset,
                             double yoffset);

  glm::vec2 mousePositionRawPx;
  std::queue<MouseEvent> events;
};

#endif
