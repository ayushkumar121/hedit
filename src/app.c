#include <app.h>
#include <config.h>

#include <gmath.h>
#include <logger.h>
#include <window.h>
#include <glyphs.h>

#include <mode.h>
#include <interface.h>

/* Global variables */
Uint width = 640;
Uint height = 480;

const Char *filePath = "buffer.txt";

/* For cursor */
Glyph cursor = {
    .ch = Cursor,
    .pos = {0.0f, 0.0f},
    .fg = {0.0f, 1.0f, 0.0f, 1.0f},
};

/* For current buffer */
FontFamily fontfamily = {.height = 16};

UIText fileText = {0};
UIText statusText = {0};
UIText commandText = {0};

UIText *activeText;

Mode curretMode = NORMAL;
Vec2 cameraOffset = {.x = 0.0f, .y = 0.0f};

void DrawStatus()
{
    char buf[100];
    sprintf(buf, "Mode: [%s], File: %s, Size: (%d/%d) bytes, %d",
            ModeString(curretMode),
            filePath,
            activeText->length,
            activeText->capacity,
            activeText->cursor);

    UITextSetBuffer(&statusText, buf, strlen(buf));
    UITextDraw(&statusText);
}

void ResizeBuffer()
{
    if (activeText->length >= activeText->capacity - 1)
    {
        UITextResize(activeText, activeText->length + 500);
    }
}

void HandleBufferInput(Char code)
{
    ResizeBuffer();

    if (activeText->cursor < activeText->length)
    {
        activeText->buffer[activeText->cursor] = code;
    }
    else
    {
        activeText->buffer[activeText->length++] = code;
    }
    activeText->cursor++;
}

void AppLoop(App *app)
{
    DrawStatus();

    if (curretMode == NORMAL)
    {
        UITextDraw(&fileText);
    }
    else if (curretMode == COMMAND)
    {
        UITextDraw(&commandText);
    }

    if (activeText->cursor < activeText->length)
    {
        cursor.pos = activeText->glyphs[activeText->cursor].pos;
    }
    else if (activeText->length > 0)
    {
        cursor.pos = AddV2(
            activeText->glyphs[activeText->length - 1].pos,
            (Vec2){10.0f, 0.0f});
    }
    else
    {
        cursor.pos = bufferOffset;
    }

    GlyphDraw(&cursor, &fontfamily);
}

void AppInit(App *app, int argc, char *argv[])
{
    FILE *file = fopen(filePath, "r");

    LogInit("hedit.logs");

    WindowInit(&app->window, width, height);
    FontsInit(&fontfamily, FontPath);
    GlyphInit(&cursor);

    UITextInit(&fileText, &fontfamily);
    UITextSetPos(&fileText, bufferOffset);
    UITextSetColor(&fileText, colors[WHITE]);

    UITextInit(&statusText, &fontfamily);
    UITextSetPos(&statusText, topbarOffset);
    UITextSetColor(&statusText, colors[GREEN]);

    UITextInit(&commandText, &fontfamily);
    UITextSetPos(&commandText, bufferOffset);
    UITextSetColor(&commandText, colors[YELLOW]);

    activeText = &fileText;

    if (file)
    {
        /* Find a better way to get size of a file */
        fseek(file, 0, SEEK_END);
        long fsize = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (fsize > fileText.capacity)
        {
            UITextResize(&fileText, fsize + 500);
        }

        fread(fileText.buffer, sizeof(char), fsize, file);
        fclose(file);

        fileText.length = fsize;
        fileText.cursor = fsize;
    }

    /* Registering callbacks */
    glfwSetKeyCallback(app->window.glfwWindow, glfwKeyCallback);
    glfwSetCharCallback(app->window.glfwWindow, glfwCharCallback);
    glfwSetFramebufferSizeCallback(app->window.glfwWindow, glfwResizeCallback);
}

void AppRun(App *app)
{
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(app->window.glfwWindow))
    {
        app->window.width = width;
        app->window.height = height;

        WindowBind(&app->window, (Vec2){0.0f, 0.0f});
        AppLoop(app);
        WindowUnbind(&app->window);
    }
}

void AppCleanup(App *app)
{
    UITextFree(&fileText);
    UITextFree(&statusText);
    UITextFree(&commandText);

    FontsCleanup(&fontfamily);
    WindowCleanup(&app->window);

    // FILE *file = fopen(filePath, "w+");

    // if (file && bufferSize > 0)
    // {
    //     fwrite(buffer, 1, bufferSize, file);
    //     fclose(file);
    //     logInfo("Buffer wrote to file");
    // }
    // else
    // {
    //     logError("Cannot write buffer to file");
    // }
    LogInfo("Resource cleanup done");
    LogClose();
}

/*
 * Handling individual key presses like backspaces and return
 */
static void
glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        if (curretMode == COMMAND)
        {
            curretMode = NORMAL;
            activeText = &fileText;
        }
        else if (curretMode == NORMAL)
        {
            curretMode = COMMAND;
            activeText = &commandText;
        }
    }
    // else if (key == GLFW_KEY_UP && GLFW_PRESS_AND_REPEAT(action))
    // {
    //     cameraOffset.y -= 10.0f;
    // }
    // else if (key == GLFW_KEY_DOWN && GLFW_PRESS_AND_REPEAT(action))
    // {
    //     cameraOffset.y += 10.0f;
    // }
    else if (key == GLFW_KEY_LEFT && GLFW_PRESS_AND_REPEAT(action))
    {
        if (activeText->cursor > 0)
            activeText->cursor--;
    }
    else if (key == GLFW_KEY_RIGHT && GLFW_PRESS_AND_REPEAT(action))
    {
        if (activeText->cursor < activeText->length)
            activeText->cursor++;
    }
    else if (key == GLFW_KEY_BACKSPACE && GLFW_PRESS_AND_REPEAT(action))
    {
        if (activeText->cursor >= activeText->length)
        {
            if (activeText->cursor > 0)
                activeText->cursor--;

            if (activeText->length > 0)
                activeText->length--;
        }
    }
    else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        if (curretMode == COMMAND)
        {
            // Execute the current command and clear current command

            activeText->length = 0;

            curretMode = NORMAL;
            activeText = &fileText;
        }
        else if (curretMode == NORMAL)
        {
            HandleBufferInput('\n');
        }
    }
}

/*
 * Handling text input
 */
static void
glfwCharCallback(GLFWwindow *window, Uint codepoint)
{
    HandleBufferInput(codepoint);
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
