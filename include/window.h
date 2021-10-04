#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
//#include <GL/freeglut.h>
#include <GLFW/glfw3.h>

#include <logger.h>

typedef struct Window
{
    // Uint width;
    // Uint height;

    // GLFW specific
    GLFWwindow *glfwWindow;
} Window;

#include <stdio.h>

#define GLFW_PRESS_AND_REPEAT(action) (action == GLFW_REPEAT || action == GLFW_PRESS)

void windowInit(Window *window, Uint width, Uint height);
void windowBind(Uint width, Uint height, V2 camera);
void windowCleanup();

#endif /* WINDOW_H */