#ifndef APP_H
#define APP_H

#include <GL/glew.h>
#define GLEW_STATIC

#include <window.h>

typedef struct App
{
    Window window;
} App;

void AppInit(App *app, int argc, char *argv[]);
void AppRun(App *app);
void AppCleanup(App *app);

/* GLFW Callbacks */
static void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void glfwCharCallback(GLFWwindow *window, Uint codepoint);
static void glfwResizeCallback(GLFWwindow *window, int newWidth, int newHeight);

#endif /* APP_H */