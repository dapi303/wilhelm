#ifndef font__h
#define font__h

#include <glm/vec2.hpp>
#include <map>

struct Character {
  unsigned int TextureID; // ID handle of the glyph texture
  glm::ivec2 Size;        // Size of glyph
  glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
  unsigned int Advance;   // Offset to advance to next glyph
};

typedef std::map<char, Character> Characters;

Characters loadFont();

#endif
