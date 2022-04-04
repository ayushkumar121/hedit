#include <glyphs.h>

/*
    Creating a 2D glyph with positions and tex-coords
*/
void glyphInit(Glyph *glyph)
{
    // if(glyph->id)
    //     return;

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
    float ty = glyph->pos.y - (fontfamily->faces[ch].bearingY + 10.0f);

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
    glUniformMatrix4fv(2, 1, GL_TRUE, translation[0]); /* Translation matrix      */
    glUniformMatrix4fv(3, 1, GL_TRUE, scale[0]);       /* Scaling matrix          */

    glUniform4f(5, glyph->fg.r, glyph->fg.g, glyph->fg.b, glyph->fg.a);
    glUniform1i(7, 0); /* Binding texture slot  */

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
                     FontFamily *fontfamily, Vec2 pos, Vec4 fg)
{
    float x = pos.x;
    float y = pos.y;

    for (size_t i = 0; i < bufferSize; i++)
    {
        glyphs[i].pos.x = x;
        glyphs[i].pos.y = y;

        glyphs[i].ch = buffer[i];
        glyphs[i].fg = fg;

        glyphDraw(&glyphs[i], fontfamily);
        x += (fontfamily->faces[buffer[i]].advance >> 6);
        
        if(buffer[i] == '\n') {
            y += fontfamily->faces[buffer[i]].height;
            x = pos.x;
        }
    }
}

/*
 * glyph cleanup from memory
 */
void glyphCleanup(Glyph *glyph)
{
    glDeleteBuffers(1, &glyph->id);
}