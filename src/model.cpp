#include "model.h"

#include <GL/glew.h>
#include <GL/glu.h>

#include <fstream>
#include <memory>
#include <sstream>

#include "loader/obj.h"
Model::~Model() {
  SDL_Log("remove model %s\n", this->name.c_str());
  this->clear();
}

void Model::render() {
  if (this->modelData) {
    glScalef(this->scale, this->scale, this->scale);
    glRotatef(this->rotate, 1, 1, 1);
    this->rotate += 0.06f;
    std::vector<std::shared_ptr<Vertex3>>::iterator vIt;
    std::vector<std::shared_ptr<Vertex2>>::iterator vtIt;
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->modelData->textureId);
    for (std::shared_ptr<Face> face : this->modelData->face) {
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
    this->test = false;
    glDisable(GL_TEXTURE_2D);
  }
}

bool Model::load(const char *path, const char *texturePath) {
  bool result = false;
  this->name = path;
  SDL_Log("loading %s\n", path);
  this->modelData = loadObj(path, texturePath);
  SDL_Log("load result: %d\n", this->modelData != nullptr);
  if (this->modelData) {
    SDL_Log("v:%d vt:%d f:%d\n", this->modelData->vertex.size(),
            this->modelData->vertexTexture.size(),
            this->modelData->face.size());

    std::vector<std::shared_ptr<Vertex3>>::reverse_iterator vIt;
    std::vector<std::shared_ptr<Vertex2>>::reverse_iterator vtIt;
  }

  return this->modelData != nullptr;
}

void Model::clear() {}
