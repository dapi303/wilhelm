#ifndef button__h
#define button__h
#include "text.h"
#include <string>

// class UiElement {
// public:
// bool click(float x, float y);
// void const render();

// protected:
// float x;
// float y;
//};

class Button {
public:
  Button(GLuint shaderId, Text &text, std::string label, int x, int y,
         float scale = 1.0f);
  bool click(int x, int y);
  void const render();

private:
  Text &text;
  std::string label;
  float x;
  float y;

  GLuint VAO;
  GLuint VBO;
  GLuint shaderId;
};

#endif
