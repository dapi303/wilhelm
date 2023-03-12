#ifndef matrices__h
#define matrices__h

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

void updateMetrices(float windowWidth, float windowHeight);
void changeFocusPoint(glm::vec3 &point);
void moveCamera(glm::vec3 &shift);
void zoomCamera(double value);

glm::vec3 getCameraPosition();
glm::mat4 getProjectionMatrix();
glm::mat4 getViewMatrix();

#endif
