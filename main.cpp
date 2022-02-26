#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics.h>
#include <locale.h>
#include <math.h>

static int framebufferWidth;
static int framebufferHeight;

static float triangleVertices[] = {
    -0.5f, -0.5f,  0.0f,
     0.5f, -0.5f,  0.0f,
     0.0f,  0.5f,  0.0f,
};

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

void glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    message("Framebuffer size: %dx%d", width, height);
    framebufferWidth = width;
    framebufferHeight = height;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // message("key: %d, scancode: %d, action: %d, mods: %d", key, scancode, action, mods);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        exit(EXIT_SUCCESS);
    } 
}

void charCallback(GLFWwindow *window, unsigned int codepoint)
{
    // message("%C", (wchar_t)codepoint);
}

void rotateTriangle()
{
    for (unsigned long i = 0; i < 9; i += 3)
    {
        double x = (double)triangleVertices[i];
        double y = (double)triangleVertices[i + 1];

        double theta = 0.0174533;

        float newX = (float)(x * cos(theta) - y * sin(theta));
        float newY = (float)(y * cos(theta) + x * sin(theta));

        triangleVertices[i] = newX;
        triangleVertices[i + 1] = newY;

        // for (unsigned long j = 0; j < 9; j++) {
            // message("triangleVertices[%d] = %f", j, triangleVertices[j]);
        // }
    }
}

int main(void)
{
    // Set locale to UTF-8 for printing unicode code points
    setlocale(LC_ALL, "en_US.UTF-8");

    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit())
        fatalError("Couldn't initialize GLFW");

    // NOTE: This snippet ensures version of OpenGL becomes 4.1. 
    // Why? How? No idea.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (!window)
        fatalError("Couldn't create a window");

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        fatalError("Couldn't initialize GLAD");

    // Log OpenGL version to stdout
    const GLubyte *glVendor = glGetString(GL_VENDOR);
    const GLubyte *glVersion = glGetString(GL_VERSION);
    message("OpenGL version %s by %s", glVersion, glVendor);

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

    // Generate vertex buffer
    GLuint vertexBufferHandle;
    glGenBuffers(1, &vertexBufferHandle);


    // Load shader
    GLuint shaderProgramHandle = glCreateProgram();
    glAttachShader(shaderProgramHandle, loadShader("assets/fragment.glsl", GL_FRAGMENT_SHADER));
    glAttachShader(shaderProgramHandle, loadShader("assets/vertex.glsl", GL_VERTEX_SHADER));
    glLinkProgram(shaderProgramHandle);

    // Create time uniform
    GLint timeUniform = glGetUniformLocation(shaderProgramHandle, "timeUniform");

    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCharCallback(window, charCallback);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the loaded shader
        glUseProgram(shaderProgramHandle);

        // Pass the current time to the shader
        glUniform1f(timeUniform, (float)glfwGetTime());


        // Buffer vertices
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), triangleVertices, GL_DYNAMIC_DRAW);


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
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(0);


        // Swap the front and back buffers
        glfwSwapBuffers(window);
        glfwPollEvents();


        // Rotate the triangle vertices around the origin
        rotateTriangle();
    }

    return 0;
}
