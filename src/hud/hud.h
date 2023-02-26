#ifndef hud__h
#define hud__h

#include "../structs.h"
#include "text.h"

class HUD {
public:
  int init(const AppWindow &appWindow);

  void render();

private:
  Text text;
};

#endif
