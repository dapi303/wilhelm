#ifndef hud__h
#define hud__h

#include "../structs.h"
#include "../fps.h"
#include "text.h"

class HUD {
public:
  HUD(Fps &fps);
  int init(const AppWindow &appWindow);
  void render();

private:
  Fps &fps;
  Text text;
};

#endif
