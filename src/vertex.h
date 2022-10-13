#ifndef vertex__h
#define vertex__h

#include <SDL2/SDL_render.h>

#include <memory>
#include <string>
#include <vector>

struct Vertex3 {
  float x;
  float y;
  float z;

  Vertex3(float x = 0.0f, float y = 0.0f, float z = 0.0f) {
    this->x = x;
    this->y = y;
    this->z = z;
  }
};

struct Vertex2 {
  float x;
  float y;

  Vertex2(float x = 0, float y = 0) {
    this->x = x;
    this->y = y;
  }
};

struct Face {
  std::vector<std::shared_ptr<Vertex3>> vertex;
  std::vector<std::shared_ptr<Vertex2>> vertexTexture;
};

struct ModelData {
  std::vector<std::shared_ptr<Vertex3>> vertex;
  std::vector<std::shared_ptr<Face>> face;
  std::vector<std::shared_ptr<Vertex2>> vertexTexture;
  unsigned int textureId;
};

#endif
