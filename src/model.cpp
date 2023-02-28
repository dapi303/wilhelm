#include "model.h"

#include <GL/glew.h>
#include <GL/glu.h>

#include <fstream>
#include <memory>
#include <sstream>

#include "loader/obj.h"

Model::Model(const ModelMeta &meta) : meta(meta), scale(meta.scale) {}

Model::~Model() {
  SDL_Log("remove model %s\n", name.c_str());
  clear();
  SDL_Log("remove model done\n");
}

const ModelMeta &Model::getMeta() { return meta; }

void Model::render() {
  // glPushMatrix();
  // glLoadIdentity();
  if (modelData) {
    glScalef(scale, scale, scale);
    std::vector<std::shared_ptr<Vertex3>>::iterator vIt;
    std::vector<std::shared_ptr<Vertex2>>::iterator vtIt;
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, modelData->textureId);
    for (std::shared_ptr<Face> face : modelData->face) {
      vIt = face->vertex.begin();
      vtIt = face->vertexTexture.begin();
      glBegin(GL_POLYGON);
      if (glGetError()) {
        SDL_Log("error:%s\n", glGetError());
      }
      while (vIt != face->vertex.end() && vtIt != face->vertexTexture.end()) {
        glTexCoord2f(vtIt->get()->x, vtIt->get()->y);
        glVertex3f(vIt->get()->x, vIt->get()->y, vIt->get()->z);
        ++vIt;
        ++vtIt;
      }
      glEnd();
    }
    glDisable(GL_TEXTURE_2D);
  }
}

bool Model::load(const char *path, const char *texturePath) {
  bool result = false;
  SDL_Log("Model->loading %s\n", path);
  if (modelData != nullptr) {
    SDL_LogError(0, "load model twice");
    return false;
  }

  name = path;
  modelData = loadObj(path, texturePath);
  SDL_Log("load result: %d\n", modelData != nullptr);
  if (modelData) {
    SDL_Log("v:%d vt:%d f:%d\n", modelData->vertex.size(),
            modelData->vertexTexture.size(), modelData->face.size());

    std::vector<std::shared_ptr<Vertex3>>::reverse_iterator vIt;
    std::vector<std::shared_ptr<Vertex2>>::reverse_iterator vtIt;
  }

  return modelData != nullptr;
}

void Model::clear() {}
