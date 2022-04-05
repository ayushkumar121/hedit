#ifndef INTERFACE_H
#define INTERFACE_H

#define DEFAULT_UITEXT_CAPACITY 100

#include <gmath.h>
#include <glyphs.h>
#include <fonts.h>

typedef struct UIText
{
    Glyph *glyphs;
    Char *buffer;

    Uint length;
    Uint capacity;
    Uint cursor;

    Vec2 pos;
    Vec4 color;
    FontFamily *fontFamily;
} UIText;

void UITextInit(UIText *uitext, FontFamily *fontFamily);
void UITextSetBuffer(UIText *uitext, Char *buffer, Uint length);
void UITextSetPos(UIText *uitext, Vec2 pos);
void UITextSetColor(UIText *uitext, Vec4 color);
void UITextResize(UIText *uitext, Uint capacity);
void UITextDraw(UIText *uitext);
void UITextFree(UIText *uitext);

#endif