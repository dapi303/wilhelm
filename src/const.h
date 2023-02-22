#ifndef const__h
#define const__h

#define CLEAR_COLOR 0.f, 0.f, 0.f, 1.f

namespace CONST {
const float minDistanceToMove = 0.003f;
const float initialObjectScale = 0.002f;

namespace camera {
const float fovy = 45.0f;
const float near = 0.1f;
const float far = 20.0f;
const float eyeY = 2.0f;
const float eyeZ = -1.0f;
const float upX = 0.0f;
const float upY = 1.0f;
const float upZ = 0.0f;
} // namespace camera
} // namespace CONST
#endif