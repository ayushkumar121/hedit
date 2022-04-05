#include <interface.h>

void UITextInit(UIText *uitext, FontFamily *fontFamily)
{
    uitext->capacity = DEFAULT_UITEXT_CAPACITY;
    uitext->fontFamily = fontFamily;

    uitext->buffer = (Char *)malloc(uitext->capacity * sizeof(Char));
    uitext->glyphs = (Glyph *)malloc(uitext->capacity * sizeof(Glyph));

    for (size_t i = 0; i < uitext->capacity; i++)
    {
        GlyphInit(&(uitext->glyphs[i]));
    }
}

void UITextSetBuffer(UIText *uitext, Char *buffer, Uint length)
{
    if (length > uitext->capacity)
    {
        UITextResize(uitext, length);
    }

    memcpy(uitext->buffer, buffer, length);
    uitext->length = length;
}

void UITextSetPos(UIText *uitext, Vec2 pos)
{   
    uitext->pos = pos;
}


void UITextSetColor(UIText *uitext, Vec4 color)
{
    uitext->color = color;
}

void UITextResize(UIText *uitext, Uint capacity)
{
    uitext->buffer = realloc(uitext->buffer, capacity * sizeof(Char));
    uitext->glyphs = realloc(uitext->glyphs, capacity * sizeof(Glyph));

    for (size_t i = uitext->capacity; i < capacity; i++)
    {
        GlyphInit(&uitext->glyphs[i]);
    }
    uitext->capacity = capacity;
}

void UITextDraw(UIText *uitext)
{
    GlyphBufferDraw(uitext->glyphs,
                    uitext->buffer,
                    uitext->length,
                    uitext->fontFamily,
                    uitext->pos,
                    uitext->color);
}

void UITextFree(UIText *uitext)
{
    for (size_t i = 0; i < uitext->capacity; i++)
    {
        GlyphCleanup(&uitext->glyphs[i]);
    }

    free(uitext->buffer);
    free(uitext->glyphs);
}