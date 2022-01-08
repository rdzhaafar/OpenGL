#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics.h>

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

    // Set clear color
    GLfloat r, g, b, a;
    r = 0.0f;
    g = 0.0f;
    b = 1.0f;
    a = 1.0f;
    glClearColor(r, g, b, a);

    // Set up the VAO.
    GLuint vertexArrayHandle;
    glGenVertexArrays(1, &vertexArrayHandle);
    glBindVertexArray(vertexArrayHandle);

    // Triangle
    const GLfloat triangleVertices[] = {
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.0f,  0.5f,  0.0f,
    };
    GLuint vertexBufferHandle;
    glGenBuffers(1, &vertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    GLuint shaderProgramHandle = glCreateProgram();
    glAttachShader(shaderProgramHandle, loadShader("assets/fragment.glsl", GL_FRAGMENT_SHADER));
    glAttachShader(shaderProgramHandle, loadShader("assets/vertex.glsl", GL_VERTEX_SHADER));
    glLinkProgram(shaderProgramHandle);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the loaded shader
        glUseProgram(shaderProgramHandle);

        // Draw the vertex array contents as triangles
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
        glVertexAttribPointer(
            0,         // attribute 0
            3,         // size
            GL_FLOAT,  // type
            GL_FALSE,  // normalized?
            0,         // stride
            NULL       // array buffer offset
        );
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        // Swap the front and back buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Exit on ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
            break;
    }

    glfwTerminate();
    return 0;
}
