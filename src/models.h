#ifndef models__h
#define models__h

#include <string>
#include <vector>

struct ModelMeta {
  std::string id;
  std::string pathObject;
  std::string pathTexture;
  float scale;
};

const std::vector<ModelMeta> modelsMeta({
    {"tree1", "media/tree/tree1.obj", "media/tree/tree.png", 1.0f},
    {"tree0", "media/tree/tree0.obj", "media/tree/tree.png", 1.0f},
    {"box", "media/player.obj", "", 0.02f},
});

#endif
