#include "matrices.h"
#include "objLoader.h"
#include <glm/ext/matrix_transform.hpp>
#include <model.h>
#include <shaders.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <text.h>

GLuint loadTexture(const std::string &path);

Model::~Model() {
  glDeleteBuffers(1, &vertexBuffer);
  glDeleteBuffers(1, &uvBuffer);
  glDeleteVertexArrays(1, &vertexArrayId);
}

void Model::load(const std::string &modelPath, const std::string &texturePath) {
  glGenVertexArrays(1, &vertexArrayId);
  glBindVertexArray(vertexArrayId);

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;
  loadOBJ(modelPath.c_str(), vertices, uvs, normals);

  verticesCount = vertices.size();

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
               &vertices[0], GL_STATIC_DRAW);

  printf("vertices %zu\n", vertices.size());
  printf("uvs %zu\n", uvs.size());
  printf("normals %zu\n", normals.size());

  glGenBuffers(1, &uvBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0],
               GL_STATIC_DRAW);

  glGenBuffers(1, &normalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0],
               GL_STATIC_DRAW);

  matrixId = glGetUniformLocation(modelShader, "MVP");
  viewMatrixId = glGetUniformLocation(modelShader, "V");
  modelMatrixId = glGetUniformLocation(modelShader, "M");
  modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

  texture = loadTexture(texturePath);
  textureId = glGetUniformLocation(modelShader, "myTextureSampler");
  lightId = glGetUniformLocation(modelShader, "LightPosition_worldspace");
}

GLuint loadTexture(const std::string &path) {

  int width, height, nrChannels;

  unsigned char *data =
      stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  GLuint textureID;
  glGenTextures(1, &textureID);

  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR,
               GL_UNSIGNED_BYTE, data);

  delete[] data;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);

  return textureID;
}

void const Model::render(const glm::mat4 &projectionMatrix,
                         const glm::mat4 &viewMatrix,
                         const glm::vec3 &translate,
                         const glm::vec3 &scale) const {
  glUseProgram(modelShader);
  glBindVertexArray(vertexArrayId);

  glm::mat4 mvp = projectionMatrix * viewMatrix *
                  glm::scale(glm::translate(modelMatrix, translate), scale);
  glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

  glm::vec3 lightPos = getCameraPosition() + glm::vec3(2, 4, 2);
  glUniform3f(lightId, lightPos.x, lightPos.y, lightPos.z);
  glm::mat4 ViewMatrix = viewMatrix;
  glm::mat4 ModelMatrix = glm::mat4(1.0);
  glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, &ModelMatrix[0][0]);
  glUniformMatrix4fv(viewMatrixId, 1, GL_FALSE, &ViewMatrix[0][0]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glUniform1i(textureId, 0);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

  glDrawArrays(GL_TRIANGLES, 0, verticesCount);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
