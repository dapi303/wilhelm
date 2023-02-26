#include "font.h"

#include <GL/glew.h>
#include <SDL_log.h>
#include <freetype/freetype.h>
#include <ft2build.h>

#include "exit_codes.h"
#include FT_FREETYPE_H

Characters loadFont() {
  FT_Error error;
  FT_Face face;
  FT_Library ft;
  FT_GlyphSlot slot;

  Characters characters;

  error = FT_Init_FreeType(&ft);
  if (error != 0) {
    // return error;
  }

  error = FT_New_Face(ft, "media/font.ttf", 0, &face);
  if (error != 0) {
    SDL_Log("loadFont, failed to create face");
    // return error;
  }

  // error = FT_Set_Char_Size(face, 50 * 64, 0, 100, 0);
  // if (error != 0) {
  // SDL_Log("loadFont, failed to set char size");
  //// return error;
  //}

  FT_Set_Pixel_Sizes(face, 0, 48);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  for (Uint32 c = 0; c < 128; ++c) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      SDL_Log("loadFont, failed to load glyph %c\n", c);
      continue;
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                 face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use
    Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<unsigned int>(face->glyph->advance.x)};
    characters.insert(std::pair<char, Character>(c, character));
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  FT_Done_Face(face);
  FT_Done_FreeType(ft);
  return characters;
  // return 0;
}
