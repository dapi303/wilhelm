#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <matrices.h>

namespace {
glm::vec3 focusPoint(0.0f, 0.0f, 0.0f);
glm::vec3 cameraShift(0.0f, 1.0f, -1.0f);
glm::vec3 camera(focusPoint + cameraShift);
glm::mat4 projection;
glm::mat4 view;
} // namespace

void updateMetrices(float windowWidth, float windowHeight) {
  projection = glm::perspective(45.0f, windowWidth / windowHeight, 0.1f, 20.0f);
  camera = focusPoint + cameraShift;
  view = glm::lookAt(camera, focusPoint, glm::vec3(0, 1, 0));
}

void changeFocusPoint(glm::vec3 &point) {
  focusPoint = point;
  camera = focusPoint + cameraShift;
  view = glm::lookAt(camera, focusPoint, glm::vec3(0, 1, 0));
}

glm::vec3 getCameraPosition() { return camera; }
glm::mat4 getProjectionMatrix() { return projection; }
glm::mat4 getViewMatrix() { return view; }
