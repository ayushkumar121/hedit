#ifndef GLYPH_H
#define GLYPH_H

#include <GL/glew.h>

#include <gmath.h>
#include <fonts.h>

typedef struct Vertex
{
    V2 pos;
    V2 tex;
} Vertex;

typedef struct Glyph
{
    V2 pos;
    V4 color;

    Uint id;
    Char ch;
} Glyph;

void glyphInit(Glyph *glyph);
void glyphDraw(Glyph *glyph, FontFamily *fonts);
void glyphCleanup(Glyph *glyph);

void glyphBufferDraw(Glyph *glyphs, Char *buffer, Uint bufferSize,
                     FontFamily *fonts, V2 pos, V4 color);

#endif /* GLYPH_H */