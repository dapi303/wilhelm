#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <stdio.h>
#include <stdlib.h>

#include <window.h>
#include <model.h>

int main(void) {
  Window window(1024, 768);
  if (window.create() != 0) {
    return -1;
  }

  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  Model model;
  model.load();

  glm::mat4 projectionMatrix = window.getProjection();
  glm::mat4 viewMatrix =
      glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    model.render(projectionMatrix, viewMatrix);

    glfwSwapBuffers(window.getInstance());
    glfwPollEvents();
  } while (glfwGetKey(window.getInstance(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window.getInstance()) == 0);

  window.close();

  return 0;
}

