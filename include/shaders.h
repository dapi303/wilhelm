#ifndef shaders__h
#define shaders__h

#include <GL/gl.h>

extern GLuint modelShader;
extern GLuint uiShader;
extern GLuint textShader;

void loadAllShaders();
GLuint loadShaders(const char *vertexPath, const char *fragmentPath);
void deleteShaders();

#endif
