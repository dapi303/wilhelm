#include "text.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

int Text::init(const AppWindow &appWindow) {
  fontChars = loadFont();
  shader = new Shader("shaders/font.vert", "shaders/font.frag");
  shader->use();
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(appWindow.width),
                                    0.0f, static_cast<float>(appWindow.height));
  glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1,
                     GL_FALSE, glm::value_ptr(projection));

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  return 0;
}

void Text::render(std::string text, float x, float y, float scale,
                  glm::vec3 color) {
  shader->use();
  glUniform3f(glGetUniformLocation(shader->ID, "textColor"), color.x, color.y,
              color.z);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);

  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    Character ch = fontChars[*c];

    float xpos = x + ch.Bearing.x * scale;
    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;
    float vertices[6][4] = {
        {xpos, ypos + h, 0.0f, 0.0f},    {xpos, ypos, 0.0f, 1.0f},
        {xpos + w, ypos, 1.0f, 1.0f},

        {xpos, ypos + h, 0.0f, 0.0f},    {xpos + w, ypos, 1.0f, 1.0f},
        {xpos + w, ypos + h, 1.0f, 0.0f}};
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(
        GL_ARRAY_BUFFER, 0, sizeof(vertices),
        vertices); // be sure to use glBufferSubData and not glBufferData

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    x += (ch.Advance >> 6) *
         scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount
                // of 1/64th pixels by 64 to get amount of pixels))
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
