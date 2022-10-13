#ifndef obj2__h
#define obj2__h

#include <SDL2/SDL_surface.h>

#include <fstream>
#include <memory>
#include <sstream>
#include <vector>

#include "../vertex.h"

bool processVertex(ModelData &model, const std::string &entry);

bool processFace(ModelData &model, const std::string &entry);

bool processVertexTexture(ModelData &model, const std::string &entry);

bool processEntry(ModelData &model, const std::string &entry);

std::shared_ptr<ModelData> loadObj(const std::string &path,
                                   const std::string &texturePath);
#endif
