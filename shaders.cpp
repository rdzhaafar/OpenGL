#include <graphics.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

static char shaderCompilationLog[1024];
static int shaderCompilationLogSize = 1024;

GLuint loadShader(char *shaderFilePath, GLenum shaderType)
{
    // Read the source text
    FILE *shaderFile = fopen(shaderFilePath, "r");
    if (!shaderFile)
        fatalError("Couldn't open shader file '%s'", shaderFilePath);

    fseek(shaderFile, 0, SEEK_END);
    size_t shaderFileSize = ftell(shaderFile);
    fseek(shaderFile, 0, SEEK_SET);

    char *shaderText = (char *)calloc(shaderFileSize + 1, 1);
    size_t read = 0;
    while (read != shaderFileSize)
        read += fread(
            shaderText + read,
            1,
            shaderFileSize - read,
            shaderFile
        );
    fclose(shaderFile);



    // Compile the source text
    GLuint shaderHandle = glCreateShader(shaderType);
    if (!shaderHandle)
        fatalError("glCreateShader returned 0. Is the shader type correct?");

    glShaderSource(shaderHandle, 1, &shaderText, NULL);
    glCompileShader(shaderHandle);

    GLint compilationResult;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compilationResult);
    if (compilationResult != GL_TRUE) {
        glGetShaderInfoLog(shaderHandle, shaderCompilationLogSize, NULL, shaderCompilationLog);
        fatalError("Couldn't successfully compile shader %s\nLog: %s", shaderFilePath, shaderCompilationLog);
    }

    return shaderHandle;
}
