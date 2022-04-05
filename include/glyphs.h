#ifndef GLYPH_H
#define GLYPH_H

#include <GL/glew.h>

#include <gmath.h>
#include <fonts.h>

typedef struct Vertex
{
    Vec2 pos;
    Vec2 tex;
} Vertex;

typedef struct Glyph
{
    Vec2 pos;
    Vec4 fg;
    Vec4 bg;

    Uint id;
    Char ch;
} Glyph;

void GlyphInit(Glyph *glyph);
void GlyphDraw(Glyph *glyph, FontFamily *fonts);
void GlyphCleanup(Glyph *glyph);

void GlyphBufferDraw(Glyph *glyphs, Char *buffer, Uint bufferSize,
                     FontFamily *fonts, Vec2 pos, Vec4 fg);

#endif /* GLYPH_H */