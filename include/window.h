#ifndef WINDOW_H
#define WINDOW_H

#include <gmath.h>
#include <shader.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct Window
{
    Shader shader;

    Uint width;
    Uint height;

    // GLFW specific
    GLFWwindow *glfwWindow;
} Window;

#define GLFW_PRESS_AND_REPEAT(action) (action == GLFW_REPEAT || action == GLFW_PRESS)

void WindowInit(Window *window, Uint width, Uint height);
void WindowBind(Window *window, Vec2 camera); // Uint width, Uint height
void WindowUnbind(Window *window);
void WindowCleanup(Window *window);

#endif /* WINDOW_H */