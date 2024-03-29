/*
 * Author : ayushkumar
 * This file abstract all the window stuff
 */

#include <window.h>

#include <logger.h>

/*
 * OpenGL debug callback
 */
void GLAPIENTRY
glErrorCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        LogError("GL Message: %s", message);
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        LogWarn("GL Message: %s", message);
        break;

    default:
        LogInfo("GL Message: %s", message);
        break;
    }
}

void glfwErrorCallback(int error, const char *description)
{
    LogError("GLFW Error :%s", description);
}

/*
 * initlizing window using GLUT
 */
void WindowInit(Window *window, Uint width, Uint height)
{
    glfwSetErrorCallback(glfwErrorCallback);
    glfwInit();

    window->glfwWindow = glfwCreateWindow(width, height, "Hedit", NULL, NULL); // glfwGetPrimaryMonitor()
    if (!window->glfwWindow)
    {
        LogError("GLFW error initialization failed");
        glfwTerminate();
    }
    LogInfo("GLFW initializationed");

    // glfwSetWindowOpacity(window->glfwWindow, 0.7f);
    glfwMakeContextCurrent(window->glfwWindow);

    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK)
    {
        LogError("GLEW error :%s", glewGetErrorString(glew_status));
    } /* GLEW Inizializing failed */

    LogInfo("GLEW_VERSION :%s", glewGetString(GLEW_VERSION));
    LogInfo("GL_VERSION   :%s", glGetString(GL_VERSION));

    /* OpenGL Error Handling */
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(glErrorCallback, NULL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    /* Enable Blending */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    ShaderInit(&window->shader);
}

/*
 * Get an orthographic projection matrix
 */
void WindowBind(Window *window, Vec2 camera)
{
    ShaderBind(&window->shader);

    float left = 0.0f;
    float right = window->width;
    float top = 0.0f;
    float bottom = window->height;
    float nearZ = -1.0f;
    float farZ = 1.0f;

    float projection[4][4] =
        {
            2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
            0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
            0.0f, 0.0f, -2.0f / (farZ - nearZ), -(farZ + nearZ) / (farZ - nearZ),
            0.0f, 0.0f, 0.0f, 1.0f};

    float view[4][4] =
        {
            1.0f, 0.0f, 0.0f, camera.x,
            0.0f, 1.0f, 0.0f, camera.y,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};

    glUniformMatrix4fv(0, 1, GL_TRUE, projection[0]);
    glUniformMatrix4fv(1, 1, GL_TRUE, view[0]);

    glClear(GL_COLOR_BUFFER_BIT);
}

void WindowUnbind(Window *window)
{
    glfwSwapBuffers(window->glfwWindow);
    glfwPollEvents();
}

void WindowCleanup(Window *window)
{
    ShadersCleanup(&window->shader);
    glfwTerminate();
}