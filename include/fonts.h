#ifndef FONTS_H
#define FONTS_H

#include <GL/glew.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <gmath.h>

typedef struct FontFace
{
    Uint id;
    
    Uint width;
    Uint height;
    
    Uint bearingX;
    Uint bearingY;

    Uint advance;
} FontFace;

typedef struct FontFamily
{
    Uint width;
    Uint height;
    
    FontFace faces[128];
} FontFamily;

void FontsInit(FontFamily *fontfamily, const Char* filepath);
void FontsCleanup(FontFamily *fontfamily);

#endif /* FONTS_H */