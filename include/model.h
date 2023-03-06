#ifndef model__h
#define model__h

#include <glm/glm.hpp>
#include <GL/glew.h>

class Model {
public:
  ~Model();
  void load();
  void render(const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix);

private:
  GLuint vertexArrayId;
  GLuint programId;
  GLuint vertexBuffer;
  GLuint colorBuffer;
  GLuint texture;
  GLuint textureId;
  GLuint matrixId;
  glm::mat4 modelMatrix;
};

#endif
