#ifndef hud__h
#define hud__h

#include "../structs.h"
#include "../fps.h"
#include "text.h"

class HUD {
public:
  HUD(Fps &fps, AppWindow &appWindow, Creature &player);
  int init();
  void render();

private:
  void renderDebug();

  Fps &fps;
  Text text;
  AppWindow &appWindow;
  Creature &player;
};

#endif
