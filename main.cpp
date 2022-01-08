#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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
    fatalError("GLFW(%d): %s", error, description);
}

int main(void)
{
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit())
        fatalError("Couldn't initialize GLFW");

    // NOTE: This snippet ensures version of OpenGL becomes 4.1. 
    // Why? How? No idea.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (!window)
        fatalError("Couldn't create a window");

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        fatalError("Couldn't initialize GLAD");

    // Log OpenGL version to stdout
    const GLubyte *glVendor = glGetString(GL_VENDOR);
    const GLubyte *glVersion = glGetString(GL_VERSION);
    message("OpenGL version %s by %s\n", glVersion, glVendor);

    // Set clear color to annoying bright pink
    GLfloat r, g, b, a;
    r = 1.0;
    g = 0.0;
    b = 0.5;
    a = 1.0;
    glClearColor(r, g, b, a);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Exit on ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
            break;
    }

    glfwTerminate();
    return 0;
}
