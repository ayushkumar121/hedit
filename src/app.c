#include <app.h>

#define topbarWidth 100 // In char length

enum Mode
{
    NORMAL,
    EDIT,
};

/* Global variables */
Uint width = 640;
Uint height = 480;

Char *filePath = "buffer.txt";
Char *logPath = "hedit.logs";
Char *fontPath = "assets/fonts/MonospaceBold.ttf";

FontFamily fontfamily = {.height = 16};
Shader shader = {0};

/* For cursor */
Int cursorIndex = 0;
Glyph cursor = {
    .ch = '_',
    .pos = {0.0f, 0.0f},
    .fg = {0.0f, 1.0f, 0.0f, 1.0f},
};
Vec2 camera = {.x = 0.0f, .y = 0.0f};

/* Topbar */
Glyph *topbar;
Char topbarText[topbarWidth];
Vec2 topbarOffset = {10.0f, 60.0f};

/* For current buffer */
Glyph *glyphs;
Char *buffer;

Uint bufferSize = 0;
Uint maxbufferSize = 60;
Vec2 bufferOffset = {10.0f, 80.0f};

Vec4 colors[] = {
    /* foreground color */
    [0] = {0.92f, 0.86f, 0.70f, 1.0f},
    [1] = {0.83f, 0.36f, 0.05f, 1.0f},
    [2] = {0.0f, 1.00f, 0.00f, 1.0f},
};

void bufferRealloc()
{
    if (bufferSize >= maxbufferSize)
    {
        maxbufferSize = bufferSize + 500;

        buffer = (Char *)realloc(buffer, maxbufferSize * sizeof(Cell));
        glyphs = (Glyph *)realloc(glyphs, maxbufferSize * sizeof(Glyph));
    }
}

/*
 * GLFW Callbacks
 */

/*
 * Handling individual key presses like backspaces and return
 */
static void
glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    /* Handling arrow keys */
    if (key == GLFW_KEY_UP && GLFW_PRESS_AND_REPEAT(action))
    {
        camera.y -= 0.05f;
    }
    if (key == GLFW_KEY_DOWN && GLFW_PRESS_AND_REPEAT(action))
    {
        camera.y += 0.05f;
    }
    if (key == GLFW_KEY_LEFT && GLFW_PRESS_AND_REPEAT(action))
    {
        if (cursorIndex > 0)
            cursorIndex--;
    }
    if (key == GLFW_KEY_RIGHT && GLFW_PRESS_AND_REPEAT(action))
    {
        if (cursorIndex < (bufferSize))
            cursorIndex++;
    }

    /* Handling Backspace */
    if (key == GLFW_KEY_BACKSPACE && GLFW_PRESS_AND_REPEAT(action))
    {
        if (cursorIndex >= bufferSize)
        {
            if (cursorIndex > 0)
                cursorIndex--;

            if (bufferSize > 0)
                bufferSize--;
        }
    }

    /* Handling Return */
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        bufferRealloc();
        buffer[bufferSize++] = '\n';
    }
}

/*
 * Handling text input
 */
static void
glfwCharCallback(GLFWwindow *window, Uint codepoint)
{
    bufferRealloc();

    if (cursorIndex < bufferSize)
    {
        buffer[cursorIndex] = codepoint;
    }
    else
    {
        buffer[bufferSize++] = codepoint;
    }
    cursorIndex++;
}

/*
 * Resize callback
 */
static void
glfwResizeCallback(GLFWwindow *window, int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;

    glViewport(0, 0, newWidth, newHeight);
}

/*
 * Main frame loop
 */
void appLoop(App *app)
{
    for (size_t i = 0; i < topbarWidth; i++)
    {
        if (!topbar[i].id)
            glyphInit(&topbar[i]);
    }

    for (size_t i = 0; i < maxbufferSize; i++)
    {
        if (!glyphs[i].id)
            glyphInit(&glyphs[i]);
    }

    sprintf(topbarText, "File: %s, Size: %d/%d bytes, %d",
            filePath,
            bufferSize,
            maxbufferSize,
            cursorIndex);

    glyphBufferDraw(topbar,
                    topbarText,
                    strlen(topbarText),
                    &fontfamily,
                    topbarOffset,
                    colors[1]);

    glyphBufferDraw(glyphs,
                    buffer,
                    bufferSize,
                    &fontfamily,
                    bufferOffset,
                    colors[0]);

    if (cursorIndex < bufferSize)
    {
        cursor.pos = glyphs[cursorIndex].pos;
    }
    else if (bufferSize > 0)
    {
        cursor.pos = addV2(glyphs[bufferSize - 1].pos, (Vec2){10.0f, 0.0f});
    }
    else
    {
        cursor.pos = bufferOffset;
    }

    glyphDraw(&cursor, &fontfamily);
}

/*
 * Initializing App and allocating memory
 */
void appInit(App *app, int argc, char *argv[])
{

    FILE *file = fopen(filePath, "r");

    /* Allocating memory */
    topbar = (Glyph *)malloc(topbarWidth * sizeof(Glyph));
    glyphs = (Glyph *)malloc(maxbufferSize * sizeof(Glyph));
    buffer = (Char *)malloc(maxbufferSize * sizeof(Char));

    logInit(logPath);
    windowInit(&app->window, width, height);
    fontsInit(&fontfamily, fontPath);
    shaderInit(&shader);
    glyphInit(&cursor);

    /* Registering callbacks */
    glfwSetKeyCallback(app->window.glfwWindow, glfwKeyCallback);
    glfwSetCharCallback(app->window.glfwWindow, glfwCharCallback);
    glfwSetFramebufferSizeCallback(app->window.glfwWindow, glfwResizeCallback);

    if (file)
    {
        /* Find a better way to get size of a file */
        fseek(file, 0, SEEK_END);
        long fsize = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (fsize > bufferSize)
        {
            buffer = (Char *)realloc(buffer, fsize);
        }
        
        fread(buffer, sizeof(char), fsize, file);
        fclose(file);

        bufferSize = fsize;
        cursorIndex = fsize;

        bufferRealloc();
    }
}

/*
 * App Main
 */
void appRun(App *app)
{
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(app->window.glfwWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        shaderBind(&shader);
        windowBind(width, height, camera);
        appLoop(app);
        shaderUnbind();

        glfwSwapBuffers(app->window.glfwWindow);
        glfwPollEvents();
    }
}

/*
 * Deleting app resources
 */
void appCleanup()
{
    for (size_t i = 0; i < maxbufferSize; i++)
    {
        glyphCleanup(&glyphs[i]);
    }
    free(glyphs);

    for (size_t i = 0; i < topbarWidth; i++)
    {
        glyphCleanup(&topbar[i]);
    }
    free(topbar);

    fontsCleanup(&fontfamily);
    shadersCleanup(&shader);
    windowCleanup();

    FILE *file = fopen(filePath, "w+");

    if (file && bufferSize > 0)
    {
        fwrite(buffer, 1, bufferSize, file);
        fclose(file);
        logInfo("Buffer wrote to file");
    }
    else
    {
        logError("Cannot write buffer to file");
    }
    free(buffer);

    logInfo("Resource cleanup done");
    logClose();
}
