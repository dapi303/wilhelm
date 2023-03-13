
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <GL/glew.h>

#include <shaders.h>
#include <text.h>

int Text::init(const int width, const int height) {
  fontChars = loadFont();
  lineHeight = 0;
  for (auto it = fontChars.begin(); it != fontChars.end(); ++it) {
    if (it->second.Size.y > lineHeight) {
      lineHeight = it->second.Size.y;
    }
  }
  glUseProgram(textShader);
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f,
                                    static_cast<float>(height));
  glUniformMatrix4fv(glGetUniformLocation(textShader, "projection"), 1,
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

  return 0;
}

void const Text::renderBelow(std::string text, float x, float y, float scale,
                             glm::vec3 color) {
  render(text, x, y - lineHeight * scale, scale, color);
}

void const Text::renderBelow(std::vector<std::string> lines, float x, float y,
                             float scale, glm::vec3 color) {
  render(lines, x, y - lineHeight * scale, scale, color);
}

void const Text::render(std::vector<std::string> lines, float x, float y,
                        float scale, glm::vec3 color) {

  for (const std::string &text : lines) {
    render(text, x, y, scale, color);
    y -= lineHeight * scale;
  }
}

void const Text::render(std::string text, float x, float y, float scale,
                        glm::vec3 color) {
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glUseProgram(textShader);
  glUniform3f(glGetUniformLocation(textShader, "textColor"), color.x, color.y,
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
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}
