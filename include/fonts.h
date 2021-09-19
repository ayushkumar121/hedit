#ifndef FONTS_H
#define FONTS_H

#include <GL/glew.h>

#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <gmath.h>
#include <logger.h>

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

void fontsInit(FontFamily *fontfamily, const Char* filepath);
void fontsCleanup(FontFamily *fontfamily);

#endif /* FONTS_H */