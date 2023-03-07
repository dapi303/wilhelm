#include "objLoader.h"
#include <glm/ext/matrix_transform.hpp>
#include <model.h>
#include <shaders.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <text.h>

GLuint loadTexture();

Model::~Model() {
  glDeleteBuffers(1, &vertexBuffer);
  glDeleteBuffers(1, &colorBuffer);
  glDeleteVertexArrays(1, &vertexArrayId);
  glDeleteProgram(programId);
}

void Model::load() {
  glGenVertexArrays(1, &vertexArrayId);
  glBindVertexArray(vertexArrayId);
  programId =
      loadShaders("shaders/model.vertexshader", "shaders/model.fragmentshader");

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;
  loadOBJ("media/cube.obj", vertices, uvs, normals);

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
               &vertices[0], GL_STATIC_DRAW);

  static const GLfloat g_uv_buffer_data[] = {
      0.000059f, 1.0f - 0.000004f, 0.000103f, 1.0f - 0.336048f,
      0.335973f, 1.0f - 0.335903f, 1.000023f, 1.0f - 0.000013f,
      0.667979f, 1.0f - 0.335851f, 0.999958f, 1.0f - 0.336064f,
      0.667979f, 1.0f - 0.335851f, 0.336024f, 1.0f - 0.671877f,
      0.667969f, 1.0f - 0.671889f, 1.000023f, 1.0f - 0.000013f,
      0.668104f, 1.0f - 0.000013f, 0.667979f, 1.0f - 0.335851f,
      0.000059f, 1.0f - 0.000004f, 0.335973f, 1.0f - 0.335903f,
      0.336098f, 1.0f - 0.000071f, 0.667979f, 1.0f - 0.335851f,
      0.335973f, 1.0f - 0.335903f, 0.336024f, 1.0f - 0.671877f,
      1.000004f, 1.0f - 0.671847f, 0.999958f, 1.0f - 0.336064f,
      0.667979f, 1.0f - 0.335851f, 0.668104f, 1.0f - 0.000013f,
      0.335973f, 1.0f - 0.335903f, 0.667979f, 1.0f - 0.335851f,
      0.335973f, 1.0f - 0.335903f, 0.668104f, 1.0f - 0.000013f,
      0.336098f, 1.0f - 0.000071f, 0.000103f, 1.0f - 0.336048f,
      0.000004f, 1.0f - 0.671870f, 0.336024f, 1.0f - 0.671877f,
      0.000103f, 1.0f - 0.336048f, 0.336024f, 1.0f - 0.671877f,
      0.335973f, 1.0f - 0.335903f, 0.667969f, 1.0f - 0.671889f,
      1.000004f, 1.0f - 0.671847f, 0.667979f, 1.0f - 0.335851f};

  glGenBuffers(1, &colorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data,
               GL_STATIC_DRAW);

  matrixId = glGetUniformLocation(programId, "MVP");
  modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  texture = loadTexture();
  textureId = glGetUniformLocation(programId, "myTextureSampler");
}

GLuint loadTexture() {

  int width, height, nrChannels;

  // stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load("media/uvtemplate.bmp", &width, &height, &nrChannels, 0);
  GLuint textureID;
  glGenTextures(1, &textureID);

  // "Bind" the newly created texture : all future texture functions will
  // modify this texture
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Give the image to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR,
               GL_UNSIGNED_BYTE, data);

  // OpenGL has now copied the data. Free our own version
  delete[] data;

  // Poor filtering, or ...
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // ... nice trilinear filtering ...
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  // ... which requires mipmaps. Generate them automatically.
  glGenerateMipmap(GL_TEXTURE_2D);

  // Return the ID of the texture we just created
  return textureID;
}

void const Model::render(const glm::mat4 &projectionMatrix,
                         const glm::mat4 &viewMatrix,
                         const glm::vec3 &translate) const {
  glUseProgram(programId);
  glBindVertexArray(vertexArrayId);

  glm::mat4 mvp =
      projectionMatrix * viewMatrix * glm::translate(modelMatrix, translate);
  glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glUniform1i(textureId, 0);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(0, // attribute 0. No particular reason for 0, but
                           // must match the layout in the shader.
                        3, // size
                        GL_FLOAT, // type
                        GL_FALSE, // normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
  );

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glVertexAttribPointer(1, // attribute. No particular reason for 1, but must
                           // match the layout in the shader.
                        3, // size
                        GL_FLOAT, // type
                        GL_FALSE, // normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
  );

  glDrawArrays(GL_TRIANGLES, 0,
               12 * 3); // 3 indices starting at 0 -> 1 triangle

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
