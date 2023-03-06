#ifndef hud__h
#define hud__h

#include "fps.h"
#include "text.h"
#include "ui/button.h"

class Hud {
public:
  Hud(Fps &fps);
  void init(const int width, const int height);
  void const render();

private:
  Text text;
  int width;
  int height;
  Fps &fps;

  std::vector<Button> buttons;

  GLint shaderId;
};

#endif
