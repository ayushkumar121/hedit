#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <gmath.h>

typedef struct Shader
{
    Uint id;
} Shader;

void shaderInit(Shader *shader);
void shaderBind(Shader *shader);
void shaderUnbind();
void shadersCleanup(Shader *shader);

#endif /* SHADER_H */