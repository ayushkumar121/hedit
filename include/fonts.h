#ifndef FONTS_H
#define FONTS_H

#include <GL/glew.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <gmath.h>

#define MAX_FONTPATH_LENGTH 500

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
    Char path[MAX_FONTPATH_LENGTH];
    Uint width;
    Uint height;
    Float lineSpace;

    FontFace faces[128];
} FontFamily;

void FontsInit(FontFamily *fontfamily);
void FontsCleanup(FontFamily *fontfamily);

#endif /* FONTS_H */