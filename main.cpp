#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void message(char *messageFormat, ...)
{
    va_list variableArgs;
    va_start(variableArgs, messageFormat);

    vfprintf(stdout, messageFormat, variableArgs);
    fprintf(stdout, "\n");

    va_end(variableArgs);
}

void fatalError(char *errorFormat, ...)
{
    va_list variableArgs;
    va_start(variableArgs, errorFormat);

    fprintf(stderr, "Error: ");
    vfprintf(stderr, errorFormat, variableArgs);
    fprintf(stderr, "\n");

    va_end(variableArgs);

    exit(1);
}

void glfwErrorCallback(int error, const char *description)
{
    fatalError("GLFW: %s", description);
}

int main(int arg_count, char *args[])
{
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit())
        fatalError("Couldn't initialize GLFW");

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (!window)
        fatalError("Couldn't create a window\n");

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        fatalError("Couldn't initialize OpenGL context\n");

    const GLubyte *glVendor = glGetString(GL_VENDOR);
    const GLubyte *glVersion = glGetString(GL_VERSION);

    message("OpenGL version %s by %s\n", glVersion, glVendor);

    // NOTE: set clear color to white
    GLfloat r, g, b, a;
    r = 1.0;
    g = 0.0;
    b = 0.5;
    a = 1.0;
    glClearColor(r, g, b, a);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Main draw loop
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
