#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "ui/button.h"

Button::Button(GLuint shaderId, Text &text, std::string label, int x, int y,
               float scale)
    : shaderId(shaderId), text(text), label(label), x(x), y(y) {

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  printf("vao %d vbo %d\n", VAO, VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  static const GLfloat vertices[] = {
      0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  };

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // glUseProgram(shaderId);

  // glEnableVertexAttribArray(0);
  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glEnableVertexAttribArray(0);
  // glBindVertexArray(0);
}

bool Button::click(int x, int y) { return false; }

void const Button::render() {

  // static const GLfloat vertices[] = {0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
  // 0.0f, 1.0f, 1.0f, 1.0f};
  // glEnableVertexAttribArray(0);
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glEnableVertexAttribArray(0);
  glm::mat4 trans = glm::mat4(1.0f);
  // trans = glm::rotate(trans, glm::radians(40.0f), glm::vec3(0.0, 0.0, 1.0));
  trans = glm::translate(trans, glm::vec3(0, 0, 0.1f));
  trans = glm::scale(trans, glm::vec3(1.0f * label.length(), 1.0f, 1.0f));

  glUseProgram(shaderId);
  unsigned int tranformLoc = glGetUniformLocation(shaderId, "transform");
  glUniformMatrix4fv(tranformLoc, 1, GL_FALSE, &trans[0][0]);

  glBindVertexArray(VAO);
  //   glColor3f(1.0f, 0.0f, 0.0f);
  //  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // text.render(label, x, y, 0.3f, glm::vec3(1));
  //  glDisableVertexAttribArray(0);

  // glEnableClientState(GL_VERTEX_ARRAY);
  // glVertexPointer(3, GL_FLOAT, 0, vertices);
}

