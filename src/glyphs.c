#include <glyphs.h>

/*
    Creating a 2D glyph with positions and tex-coords
*/
void glyphInit(Glyph *glyph)
{
    Vertex vertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,

        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f};

    /* Vertex Array Object */
    glGenVertexArrays(1, &glyph->id);
    glBindVertexArray(glyph->id);

    /* Vertex Buffer */
    GLuint vb;

    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos)); /* Vertex position */
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex)); /* Vertex tex coord */

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/*
 * Drawing glyph
 */
void glyphDraw(Glyph *glyph, FontFamily *fontfamily)
{
    char ch = glyph->ch ? glyph->ch : ' ';

    float tx = glyph->pos.x + fontfamily->faces[ch].bearingX;
    float ty = glyph->pos.y - (fontfamily->faces[ch].bearingY);

    float sx = fontfamily->faces[glyph->ch].width;
    float sy = fontfamily->faces[glyph->ch].height;

    /* Model Transformations */
    float translation[4][4] =
        {
            1.0f, 0.0f, 0.0f, tx,
            0.0f, 1.0f, 0.0f, ty,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};

    float scale[4][4] =
        {
            sx, 0.0f, 0.0f, 0.0f,
            0.0f, sy, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};

    /* Binding Uniforms */
    glUniformMatrix4fv(1, 1, GL_TRUE, translation[0]); /* Translation matrix      */
    glUniformMatrix4fv(2, 1, GL_TRUE, scale[0]);       /* Scaling matrix          */

    glUniform4f(3, glyph->color.r, glyph->color.g, glyph->color.b, glyph->color.a);
    glUniform1i(4, 0); /* Binding texture slot  */

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontfamily->faces[ch].id);

    glBindVertexArray(glyph->id);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

/*
 * Drawing an entire buffer
 */
void glyphBufferDraw(Glyph *glyphs, Char *buffer, Uint bufferSize,
                     FontFamily *fontfamily, V2 pos, V4 color)
{
    float x = pos.width;
    for (size_t i = 0; i < bufferSize; i++)
    {
        glyphs[i].pos.x = x;
        glyphs[i].pos.y = pos.y;

        glyphs[i].ch = buffer[i];
        glyphs[i].color = color;

        glyphDraw(&glyphs[i], fontfamily);
        x += (fontfamily->faces[buffer[i]].advance >> 6);
    }
}

/*
 * glyph cleanup from memory
 */
void glyphCleanup(Glyph *glyph)
{
    glDeleteBuffers(1, &glyph->id);
}