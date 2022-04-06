#include <app.h>
#include <config.h>

#include <gmath.h>
#include <logger.h>
#include <window.h>

#include <mode.h>
#include <interface.h>

/* Global variables */
Uint width = 640;
Uint height = 480;

// const Char *filePath = "buffer.txt";
FontFamily fontfamily = {
    .path = FontPath,
    .height = FontHeight,
    .lineSpace = FontLineSpace,
};

UIText fileText = {0};
UIText statusText = {0};
UIText commandText = {0};
UIText cursorText = {0};

UIText *activeText;

Mode curretMode = NORMAL;
// Vec2 cameraOffset = {.x = 0.0f, .y = 0.0f};

void ResizeBuffer()
{
    if (activeText->length >= activeText->capacity - 1)
    {
        UITextResize(activeText, activeText->length + 500);
    }
}

void HandleUIInput(Char code)
{
    ResizeBuffer();

    /*
     * In between text editing
     */
    if (activeText->cursor < activeText->length)
    {
        Char temp = activeText->buffer[activeText->cursor];
        activeText->buffer[activeText->cursor] = code;

        memcpy(activeText->buffer + activeText->cursor + 2,
               activeText->buffer + activeText->cursor + 1, activeText->capacity - activeText->cursor);

        activeText->buffer[activeText->cursor + 1] = temp;

        activeText->length++;
    }
    /*
     * When cursor is ahed of text
     */
    else
    {
        activeText->buffer[activeText->length++] = code;
    }
    activeText->cursor++;
}

void DrawStatus()
{
    char buf[100];
    sprintf(buf, "Mode: [%s], Size: (%d/%d) bytes, %d",
            ModeString(curretMode),
            // filePath,
            activeText->length,
            activeText->capacity,
            activeText->cursor);

    UITextSetBuffer(&statusText, buf, strlen(buf));
    UITextDraw(&statusText);
}

void DrawCursor()
{
    /*
     * When cursor is between text
     */
    if (activeText->cursor < activeText->length)
    {
        Glyph g = activeText->glyphs[activeText->cursor];
        Vec2 pos = AddV2(g.pos, V2(0.0f - fontfamily.faces[g.ch].width / 2, 0.0f));

        UITextSetPos(&cursorText, pos);
    }
    /*
     * When cursor is ahead of text
     */
    else if (activeText->length > 0)
    {
        Glyph g = activeText->glyphs[activeText->length - 1];
        Vec2 pos = AddV2(g.pos, V2(fontfamily.faces[g.ch].width / 2, 0.0f));

        UITextSetPos(&cursorText, pos);
    }
    /*
     * When text is empty
     */
    else
    {
        Glyph g = cursorText.glyphs[0];
        Vec2 pos = AddV2(bufferOffset, V2(0.0f - fontfamily.faces[g.ch].width / 2, 0.0f));

        UITextSetPos(&cursorText, bufferOffset);
    }
    UITextDraw(&cursorText);
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

    DrawCursor();
}

void AppInit(App *app, int argc, char *argv[])
{
    LogInit("hedit.logs");

    WindowInit(&app->window, width, height);
    FontsInit(&fontfamily);

    UITextInit(&fileText, &fontfamily);
    UITextSetPos(&fileText, bufferOffset);
    UITextSetColor(&fileText, colors[WHITE]);

    UITextInit(&statusText, &fontfamily);
    UITextSetPos(&statusText, topbarOffset);
    UITextSetColor(&statusText, colors[GREEN]);

    UITextInit(&commandText, &fontfamily);
    UITextSetPos(&commandText, bufferOffset);
    UITextSetColor(&commandText, colors[YELLOW]);

    UITextInit(&cursorText, &fontfamily);
    UITextSetBuffer(&cursorText, "|", 1);
    UITextSetColor(&cursorText, colors[GREEN]);

    activeText = &fileText;

    // FILE *file = fopen(filePath, "r");
    // if (file)
    // {
    //     /* Find a better way to get size of a file */
    //     fseek(file, 0, SEEK_END);
    //     long fsize = ftell(file);
    //     fseek(file, 0, SEEK_SET);

    //     if (fsize > fileText.capacity)
    //     {
    //         UITextResize(&fileText, fsize + 500);
    //     }

    //     fread(fileText.buffer, sizeof(char), fsize, file);
    //     fclose(file);

    //     fileText.length = fsize;
    //     fileText.cursor = fsize;
    // }

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
    UITextFree(&cursorText);

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
        if (activeText->cursor > 0)
        {
            /*
             * When cursor is between text
             */
            if (activeText->cursor < activeText->length)
            {
                memcpy(activeText->buffer + activeText->cursor - 1,
                       activeText->buffer + activeText->cursor, activeText->capacity - activeText->cursor);
            }

            activeText->cursor--;
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
            HandleUIInput('\n');
        }
    }
}

/*
 * Handling text input
 */
static void
glfwCharCallback(GLFWwindow *window, Uint codepoint)
{
    HandleUIInput(codepoint);
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
