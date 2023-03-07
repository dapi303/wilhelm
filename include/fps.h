#ifndef fps__h
#define fps__h

class Fps {
public:
  void update();
  void init();
  float const getDeltaTime();
  float const getFps();

private:
  double lastUpdateS;
  double lastCounterUpdateS;
  float deltaTimeS;
  float fps;
  float minFrameTimeMs;
};

#endif
