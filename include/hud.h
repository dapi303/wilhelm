#ifndef hud__h
#define hud__h

#include "text.h"

class Hud {
public:
  void init(const int width, const int height);
  void const render();

private:
  Text text;
  int width;
  int height;
};

#endif
