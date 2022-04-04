#ifndef APP_H
#define APP_H

#include <GL/glew.h>
#define GLEW_STATIC

#include <gmath.h>
#include <logger.h>
#include <window.h>
#include <shader.h>
#include <glyphs.h>

#include <time.h>

typedef struct App
{
    Window window;
} App;

void appInit(App *app, int argc, char *argv[]);
void appRun(App  *app);
void appCleanup();

#endif /* APP_H */