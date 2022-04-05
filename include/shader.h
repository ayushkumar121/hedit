#ifndef SHADER_H
#define SHADER_H

#include <gmath.h>

typedef struct Shader
{
    Uint id;
} Shader;

void ShaderInit(Shader *shader);
void ShaderBind(Shader *shader);
void ShaderUnbind();
void ShadersCleanup(Shader *shader);

#endif /* SHADER_H */