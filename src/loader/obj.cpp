#include "./obj.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <SDL2/SDL_image.h>

#include "./stb_image.h"

bool processVertex(ModelData &model, const std::string &entry) {
  bool result = false;
  if (entry.size() >= 7) { // v x y z
    std::stringstream ss(entry);
    std::string prefix;

    std::shared_ptr<Vertex3> vertex(new Vertex3());
    model.vertex.push_back(vertex);

    if (ss >> prefix >> vertex->x >> vertex->y >> vertex->z) {
      result = true;
    }
  }
  return result;
}

bool processFace(ModelData &model, const std::string &entry) {
  bool result = false;
  if (entry.size() >= 7) {
    std::stringstream ss(entry);
    std::string prefix;

    std::shared_ptr<Face> face(new Face());
    model.face.push_back(face);
    if (ss >> prefix) {
      std::string word;
      while (ss >> word) {
        std::stringstream ssWord(word);
        int v;
        if (ssWord >> v && v > 0 && v <= model.vertex.size()) {
          face->vertex.push_back(model.vertex[v - 1]);
        } else {
          return false;
        }

        char separator;
        int vt;
        if (ssWord >> separator >> vt) {
          if (separator == '/' && vt > 0 && vt <= model.vertexTexture.size()) {
            face->vertexTexture.push_back(model.vertexTexture[vt - 1]);
          } else {
            return false;
          }
        }
      }
      result = face->vertex.size() > 2;
    }
  }
  return result;
}

bool processVertexTexture(ModelData &model, const std::string &entry) {
  bool result = false;
  if (entry.size() > 9) { // 'vt 0.x 0.y'
    std::stringstream ss(entry);
    std::string prefix;

    std::shared_ptr<Vertex2> vertex(new Vertex2());
    model.vertexTexture.push_back(vertex);
    if (ss >> prefix >> vertex->x >> vertex->y) {
      result = true;
    }
  }
  return result;
}

bool processEntry(ModelData &model, const std::string &entry) {
  bool result = true;
  if (entry.size() > 2) {
    if (entry[0] == 'v' && entry[1] == ' ') {
      result = processVertex(model, entry);
    } else if (entry[0] == 'f' && entry[1] == ' ') {
      result = processFace(model, entry);
    } else if (entry[0] == 'v' && entry[1] == 't' && entry[2] == ' ') {
      result = processVertexTexture(model, entry);
    }
  }
  return result;
}

std::shared_ptr<ModelData> loadObj(const std::string &path,
                                   const std::string &texturePath) {
  SDL_Log("loading obj %s\n", path.c_str());
  std::shared_ptr<ModelData> model(new ModelData());
  std::ifstream file(path);
  bool failure = false;
  int counter = 0;

  try {
    if (file.is_open()) {
      std::vector<std::string> lines;
      std::string line;
      while (std::getline(file, line) && !failure) {
        failure = !processEntry(*model, line);
        if (failure) {
          SDL_Log("not failed:%d\n", counter);
          SDL_Log("failed for:\n%s\n", line.c_str());
          SDL_Log("vertex:%d\n", model->vertex.size());
          SDL_Log("vt:%d\n", model->vertexTexture.size());
        } else {
          ++counter;
        }
      }

      SDL_Log("vertex:%d\n", model->vertex.size());
      SDL_Log("vt:%d\n", model->vertexTexture.size());
      file.close();
    } else {
      failure = true;
      SDL_Log("failed to open\n");
    }
  } catch (std::ios_base::failure &e) {
    SDL_Log("%s\n", e.what());
  }

  SDL_Log("reading done:%d\n", counter);

  if (!failure) {
    SDL_Log("loading texture: %s\n", texturePath.c_str());

    glGenTextures(1, &model->textureId);
    glBindTexture(GL_TEXTURE_2D, model->textureId);
    glEnable(GL_CULL_FACE);
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data =
        stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
      GLint format = nrChannels == 4 ? GL_RGBA : GL_RGB;
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      SDL_Log("texture loaded\n");
    } else {
      SDL_Log("failed\n");
    }
    stbi_image_free(data);
  }

  return failure ? nullptr : model;
}
