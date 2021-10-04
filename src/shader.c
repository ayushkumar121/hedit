/*
 * Author : ayushkumar
 * This file abstract all the shader stuff
 */

#include <shader.h>

unsigned int 
createShader(GLenum type, const char *source)
{
    unsigned int id = glCreateShader(type);

    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    return id;
}

/*
 * Creating and compiling shader program
 */
void 
shaderInit(Shader *shader)
{
    /* Shader Program */
    shader->id = glCreateProgram();

    /* Veretx Shader */
    const char *vs_source =
        "#version 430 core\n"
        "layout(location = 0) in vec4 pos;"
        "layout(location = 1) in vec2 tex;"

        "layout(location = 0) uniform mat4 projection;"
        "layout(location = 1) uniform mat4 view;"
        "layout(location = 2) uniform mat4 translation;"
        "layout(location = 3) uniform mat4 scale;"

        "out vec2 v_TexCoord;"
        "void main()"
        "{"
        "   gl_Position = view * projection * (translation * scale) * pos;"
        "   v_TexCoord  = tex;"
        "}";

    unsigned int vs = createShader(GL_VERTEX_SHADER, vs_source);

    /* Fragment Shader */
    const char *fs_source =
        "#version 430 core\n"
        "layout(location = 5) uniform vec4 u_Color;"
        "layout(location = 6) uniform sampler2D u_Texture;"
        "in vec2 v_TexCoord;"
        "out vec4 fragColor;"
        "void main()"
        "{"
        "   vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Texture, v_TexCoord).r);"
        "   fragColor = sampled * u_Color;"
        "}";

    unsigned int fs = createShader(GL_FRAGMENT_SHADER, fs_source);

    glAttachShader(shader->id, vs);
    glAttachShader(shader->id, fs);

    glLinkProgram(shader->id);
    glValidateProgram(shader->id);

    /* Deleting the redundant source strings */
    glDeleteShader(vs);
    glDeleteShader(fs);
}

/*
 * Binding the shader
 */
void 
shaderBind(Shader *shader)
{
    glUseProgram(shader->id);
}

/*
 * Unbinding the shader
 */
void 
shaderUnbind()
{
    glUseProgram(0);
}

/*
 * Shader cleanup
 */
void 
shadersCleanup(Shader *shader)
{
    glDeleteProgram(shader->id);
}