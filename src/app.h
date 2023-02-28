#ifndef app__h
#define app__h

#include <SDL2/SDL.h>

#include <vector>
#include "fps.h"
#include "glm/glm.hpp"

#include "font.h"
#include "hud/hud.h"
#include "model.h"
#include "shaders/shaders.h"
#include "structs.h"
#include "models.h"

class App {
public:
  App();
  ~App();

  int init(int width, int height);
  void loop();

private:
  int initVideo(int width, int height);
  int initGL();

  void clearObjects();

  void prepareSceneView();
  void renderObjects();
  bool loadModel(const ModelMeta &meta);

  void createObject(Position, std::string modelId);
  void updateCreature(Creature &creature, float deltaTime);
  void events();
  void click();

  SDL_Window *window = nullptr;
  SDL_Surface *screenSurface = nullptr;
  bool quit;

  Object *object = nullptr;
  Creature player;

  std::vector<std::shared_ptr<Model>> models;

  Position clickPos;
  Position clickPos2;

  Position mouse;

  HUD hud;
  AppWindow appWindow;
  bool light = true;
  bool light0 = true;
  Fps fps;
};

#endif
