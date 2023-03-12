#include <cstdio>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <matrices.h>

namespace {
#define MIN_CAMERA_SHIFT_Y 0.1f
#define MAX_CAMERA_SHIFT_Z -0.1f

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

void moveCamera(glm::vec3 &shift) {
  glm::vec3 newCameraShift = cameraShift + shift;
  if (newCameraShift.y < MIN_CAMERA_SHIFT_Y) {
    newCameraShift.y = MIN_CAMERA_SHIFT_Y;
  }
  if (newCameraShift.z > MAX_CAMERA_SHIFT_Z) {
    newCameraShift.z = MAX_CAMERA_SHIFT_Z;
  }
  cameraShift = newCameraShift;

  printf("cameraShift: %f %f %f\n", cameraShift.x, cameraShift.y,
         cameraShift.z);
  camera = focusPoint + cameraShift;
  view = glm::lookAt(camera, focusPoint, glm::vec3(0, 1, 0));
}

void zoomCamera(double value) {
  glm::vec3 step(0, -value, value);
  moveCamera(step);
}

glm::vec3 getCameraPosition() { return camera; }
glm::mat4 getProjectionMatrix() { return projection; }
glm::mat4 getViewMatrix() { return view; }
