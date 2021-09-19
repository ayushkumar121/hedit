#include <fonts.h>

/*
 * Initializing fonts and loading texture into OpenGL
 * and performing basic mip-mapping
 */
void fontsInit(FontFamily *fontfamily, const Char *filepath)
{
    FT_Library ft;
    FT_Face face;

    if (FT_Init_FreeType(&ft))
    {
        logError("FREETYPE: Could not init FreeType Library");
    }
    if (FT_New_Face(ft, filepath, 0, &face))
    {
        logError("FREETYPE: Failed to load font");
    }

    FT_Set_Pixel_Sizes(face, fontfamily->width, fontfamily->height);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (size_t c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            logError("FREETYTPE: Failed to load Glyph");
            continue;
        }
        // generate texture
        Uint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);

        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // now store character for later use
        FontFace ff = {
            .id = texture,

            .width = face->glyph->bitmap.width,
            .height = face->glyph->bitmap.rows,

            .bearingX = face->glyph->bitmap_left,
            .bearingY = face->glyph->bitmap_top,

            .advance = face->glyph->advance.x};

        fontfamily->faces[c] = ff;
    }
}

/*
 * Cleaning up fonts resources and texture cleanup
 */
void fontsCleanup(FontFamily *fonts)
{
    for (size_t i = 0; i < 128; i++)
    {
        glDeleteTextures(1, &fonts->faces[i].id);
    }
}