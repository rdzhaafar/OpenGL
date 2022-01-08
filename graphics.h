#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glad/glad.h>

void fatalError(char *errorFormat, ...);
void message(char *messageFormat, ...);

GLuint loadShader(char *shaderFilePath, GLenum shaderType);

#endif