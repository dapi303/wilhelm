#ifndef fps__h
#define fps__h

#include <SDL_stdinc.h>
class Fps {
public:
  void frameStart();
  void frameEnd();
  void init();
  void const render();
  float const getDeltaTime();
  float const getFps();

private:
  Uint32 lastUpdate;
  Uint64 currentFrameStart;
  Uint32 lastCounterUpdate;
  float deltaTime;
  float fps;
  float minFrameTimeMs;
};

#endif
