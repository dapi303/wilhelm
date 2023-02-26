#ifndef hud__h
#define hud__h

#include "../structs.h"
#include "../fps.h"
#include "text.h"

class HUD {
public:
  HUD(Fps &fps, AppWindow &appWindow);
  int init();
  void render();

private:
  Fps &fps;
  Text text;
  AppWindow &appWindow;
};

#endif
