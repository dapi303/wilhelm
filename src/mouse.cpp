#include <GL/glu.h>
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/ext/vector_float3.hpp>
#include <mouse.h>

glm::vec2 translateFromPixelPosition(glm::vec2 pixelPosition) {
  GLint view[4];
  glGetIntegerv(GL_VIEWPORT, view);
  return glm::vec2(pixelPosition.x / (view[2] * 0.5f) - 1.0f,
                   1.0f - (pixelPosition.y / (view[3] * 0.5f)));
};

glm::vec3 const Mouse::translateToWorldCords(glm::vec2 pxPosition) {
  glm::vec2 pos = translateFromPixelPosition(pxPosition);

  glm::vec4 clipCoords = glm::vec4(pos.x, pos.y, -1.0f, 1.0f);
  glm::mat4 inverseProj = glm::inverse(projectionMatrix);

  glm::vec4 eyeCoords = inverseProj * clipCoords;

  eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

  glm::mat4 inverseView = glm::inverse(viewMatrix);

  glm::vec4 rayWorld = inverseView * eyeCoords;
  glm::vec3 rayDirection =
      glm::normalize(glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z));

  glm::vec3 intersectionPoint =
      (-cameraPosition.y / rayDirection.y) * rayDirection + cameraPosition;

  return glm::vec3(intersectionPoint.x, intersectionPoint.y,
                   intersectionPoint.z);
};

Mouse::Mouse(GLFWwindow *window, glm::vec3 cameraPosition,
             glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
    : cameraPosition(cameraPosition), projectionMatrix(projectionMatrix),
      viewMatrix(viewMatrix), mousePositionRawPx(0.0f) {

  glfwSetWindowUserPointer(window, this);

  glfwSetMouseButtonCallback(window, clickCallback);
  glfwSetCursorPosCallback(window, moveCallback);
}

void Mouse::moveCallback(GLFWwindow *window, double xpos, double ypos) {
  void *data = glfwGetWindowUserPointer(window);
  Mouse *instance = static_cast<Mouse *>(data);
  instance->mousePositionRawPx.x = xpos;
  instance->mousePositionRawPx.y = ypos;
}

void Mouse::clickCallback(GLFWwindow *window, int button, int action,
                          int mods) {}

glm::vec3 const Mouse::getMouse3DPosition() {
  return translateToWorldCords(mousePositionRawPx);
}
