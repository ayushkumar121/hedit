#include <app.h>
#include <config.h>

FontFamily fontfamily = {.height = 16};
Shader shader = {0};

/* Global variables */
Uint width = 640;
Uint height = 480;

Char *filePath = "buffer.txt";

/* For cursor */
Glyph cursor = {
    .ch = Cursor,
    .pos = {0.0f, 0.0f},
    .fg = {0.0f, 1.0f, 0.0f, 1.0f},
};

Int cursorIndex = 0;
Vec2 cameraOffset = {.x = 0.0f, .y = 0.0f};

/* Topbar */
Glyph *topbar;
Char topbarText[TopbarWidth];

/* For current buffer */
Glyph *glyphs;
Char *buffer;

Uint bufferSize = 0;
Uint maxbufferSize = 60;

void bufferRealloc()
{
    if (bufferSize >= maxbufferSize - 1)
    {
        maxbufferSize = bufferSize + 500;

        buffer = (Char *)realloc(buffer, maxbufferSize * sizeof(Char));
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
        cameraOffset.y -= 10.0f;
    }
    if (key == GLFW_KEY_DOWN && GLFW_PRESS_AND_REPEAT(action))
    {
        cameraOffset.y += 10.0f;
    }
    if (key == GLFW_KEY_LEFT && GLFW_PRESS_AND_REPEAT(action))
    {
        if (cursorIndex > 0)
            cursorIndex--;
    }
    if (key == GLFW_KEY_RIGHT && GLFW_PRESS_AND_REPEAT(action))
    {
        if (cursorIndex < bufferSize)
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

        if (cursorIndex < bufferSize)
        {
            buffer[cursorIndex] = '\n';
        }
        else
        {
            buffer[bufferSize++] = '\n';
        }
        cursorIndex++;
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
    for (size_t i = 0; i < TopbarWidth; i++)
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

    glyphBufferDraw(glyphs,
                    buffer,
                    bufferSize,
                    &fontfamily,
                    addV2(bufferOffset, cameraOffset),
                    colors[YELLOW]);

    glyphBufferDraw(topbar,
                    topbarText,
                    strlen(topbarText),
                    &fontfamily,
                    topbarOffset,
                    colors[WHITE]);

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
    topbar = (Glyph *)malloc(TopbarWidth * sizeof(Glyph));
    buffer = (Char *)malloc(maxbufferSize * sizeof(Char));
    glyphs = (Glyph *)malloc(maxbufferSize * sizeof(Glyph));

    logInit("hedit.logs");
    windowInit(&app->window, width, height);
    fontsInit(&fontfamily, FontPath);
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

        if (fsize > maxbufferSize)
        {
            maxbufferSize = fsize + 500;
            buffer = (Char *)realloc(buffer, maxbufferSize * sizeof(Char));
            glyphs = (Glyph *)realloc(glyphs, maxbufferSize * sizeof(Glyph));
        }

        fread(buffer, sizeof(char), fsize, file);
        fclose(file);

        bufferSize = fsize;
        cursorIndex = fsize;
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
        windowBind(width, height, (Vec2){0.0f, 0.0f});
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

    for (size_t i = 0; i < TopbarWidth; i++)
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
