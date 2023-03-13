#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <stdio.h>
#include <stdlib.h>

#include "mouse.h"
#include "window.h"
#include "model.h"
#include "fps.h"
#include "hud.h"
#include "matrices.h"

int main(void) {
  const int width = 1024;
  const int height = 768;
  Window window(width, height);
  if (window.create() != 0) {
    return -1;
  }

  loadAllShaders();

  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  Model modelTree, modelCube;
  modelTree.load("media/tree/tree101.obj", "media/tree/tree.bmp");
  modelCube.load("media/cube.obj", "media/uvtemplate.bmp");

  glm::vec3 myPosition(0.0f);
  updateMetrices(width, height);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  Fps fps;
  Hud hud(fps);

  fps.init();
  hud.init(width, height);

  Mouse mouse(window.getInstance());

  do {
    fps.update();

    if (mouse.clicked) {
      mouse.clicked = false;
      myPosition = mouse.getMouse3DPosition();
      changeFocusPoint(myPosition);
    }
    while (std::optional<MouseEvent> event = mouse.popEvent()) {
      if (event->type == SCROLL) {
        if (event->y != 0) {
          zoomCamera(event->y * 0.1f);
        }
      }
    }

    glm::vec3 cameraPosition = getCameraPosition();
    glm::mat4 projectionMatrix = getProjectionMatrix();
    glm::mat4 viewMatrix = getViewMatrix();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    modelTree.render(projectionMatrix, viewMatrix, glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(1.00f));

    modelCube.render(projectionMatrix, viewMatrix, glm::vec3(1.0f, 0.0f, 1.0f),
                     glm::vec3(0.05f));

    modelCube.render(projectionMatrix, viewMatrix, mouse.getMouse3DPosition(),
                     glm::vec3(0.01f));

    hud.render();

    glfwSwapBuffers(window.getInstance());
    glfwPollEvents();
  } while (glfwGetKey(window.getInstance(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window.getInstance()) == 0);

  deleteShaders();
  window.close();

  return 0;
}

