#ifndef model__h
#define model__h

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include <memory>
#include <string>
#include <vector>

#include "const.h"
#include "vertex.h"

#include "models.h"

class Model {
public:
  Model(const ModelMeta &meta);
  ~Model();
  bool load(const char *path, const char *texturePath);
  void render();
  float scale = CONST::initialObjectScale;
  const ModelMeta &getMeta();

private:
  void clear();
  std::string name;

  std::shared_ptr<ModelData> modelData;
  ModelMeta meta;
};

#endif
