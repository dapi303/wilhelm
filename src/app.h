#ifndef app__h
#define app__h

#include <SDL2/SDL.h>

#include <vector>

#include "model.h"
#include "structs.h"

class App {
public:
  App();
  ~App();

  SDL_Window *getWindow();

  int init(int width, int height);
  void loop();

private:
  int initVideo(int width, int height);
  int initGL();
  void initGrid();

  void clearObjects();
  void clearGrid();

  void render();
  void renderGrid();
  void renderObjects();
  void renderColorMesh();
  void renderWorld();

  Object *createObject(Position);
  void updateCreature(Creature &creature, float deltaTime);
  void events();
  void click();

  SDL_Window *window = nullptr;
  SDL_Surface *screenSurface = nullptr;
  bool quit;

  Object *object = nullptr;
  Creature player;
  GridTile *grid = nullptr;

  std::vector<Model *> models;

  float elapsed = 0.01f;

  int width;
  int height;

  bool enableColorMesh = false;
  Position clickPos;
  Position clickPos2;

  Position mouse;
  bool enableObjects = true;
};

#endif
