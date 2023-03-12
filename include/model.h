#ifndef model__h
#define model__h

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>

class Model {
public:
  ~Model();
  void load(const std::string &modelPath, const std::string &texturePath);
  void const render(const glm::mat4 &projectionMatrix,
                    const glm::mat4 &viewMatrix, const glm::vec3 &translate,
                    const glm::vec3 &scale) const;

private:
  GLuint vertexArrayId;
  GLuint programId;
  GLuint vertexBuffer;
  GLuint colorBuffer;
  GLuint texture;
  GLuint textureId;
  GLuint matrixId;
  glm::mat4 modelMatrix;

  GLsizei verticesCount;
};

#endif
