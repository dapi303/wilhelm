#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <stdio.h>
#include <stdlib.h>

#include "window.h"
#include "model.h"
#include "fps.h"
#include "hud.h"

int main(void) {
  const int width = 1024;
  const int height = 768;
  Window window(width, height);
  if (window.create() != 0) {
    return -1;
  }

  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  Model model;
  model.load();

  glm::mat4 projectionMatrix = window.getProjection();
  glm::mat4 viewMatrix =
      glm::lookAt(glm::vec3(0, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  Fps fps;
  Hud hud(fps);

  fps.init();
  hud.init(width, height);

  do {
    fps.update();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    model.render(projectionMatrix, viewMatrix, glm::vec3(2.0f, 0.0f, 0.0f));
    model.render(projectionMatrix, viewMatrix, glm::vec3(-2.2f, 0.0f, -1.0f));

    hud.render();

    glfwSwapBuffers(window.getInstance());
    glfwPollEvents();
  } while (glfwGetKey(window.getInstance(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window.getInstance()) == 0);

  window.close();

  return 0;
}

