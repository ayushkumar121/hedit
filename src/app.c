#include <app.h>

#define topbarWidth 100 // In char length

/* Global variables */
Uint width = 640;
Uint height = 480;

Char *filePath = "buffer.txt";
Char *logPath = "hedit.logs";
Char *fontPath = "assets/fonts/Cascadia.ttf";

FontFamily fontfamily = {.height = 16};
Shader shader = {0};

/* For cursor */
Int cursorIndex = 0;
Int col = 0;
Int row = 0;

Glyph cursor = {.ch = '_', .color = {.r = 0.0f, .g = 1.0f, .b = 0.0f, .a = 1.0f}};

/* For current buffer */
Uint bufferSize = 0;
Uint maxbufferSize = 100;

Cell *cells;
Glyph *glyphs;
V2 offset = {.x = 10.0f, .y = 50.0f};
V4 colors[] =
    {
        /* foreground color */
        [0] = {.r = 0.92f, .g = 0.86f, .b = 0.70f, .a = 1.0f},
        [1] = {.r = 0.83f, .g = 0.36f, .b = 0.05f, .a = 1.0f},
        [2] = {.r = 0.0f, .g = 1.0f, .b = 0.0f, .a = 1.0f}};

V2 camera = {.x = 0.0f, .y = 0.0f};

/* Topbar */
Glyph *topbar;
V2 topbaroffset = {.x = 10.0f, .y = 20.0f};

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
        camera.y += 0.05f;
    }
    if (key == GLFW_KEY_DOWN && GLFW_PRESS_AND_REPEAT(action))
    {
        camera.y -= 0.05f;
    }
    if (key == GLFW_KEY_LEFT && GLFW_PRESS_AND_REPEAT(action) && cursorIndex > 0)
    {
        cursorIndex--;
        col--;
    }
    if (key == GLFW_KEY_RIGHT && GLFW_PRESS_AND_REPEAT(action))
    {
        cursorIndex++;
        col++;
    }

    /* Handling Backspace */
    if (key == GLFW_KEY_BACKSPACE && GLFW_PRESS_AND_REPEAT(action) && bufferSize > 0)
    {
        // if (cursorIndex < (Int)bufferSize && cursorIndex >= 0)
        // {
        //     Uint k = cursorIndex--;
        //     Cell kcell = cells[k];
        //     Cell knext = cells[k + 1];

        //     for (size_t i = k; i < bufferSize - 1; i++)
        //     {
        //         cells[i] = cells[i + 1];

        //         if (cells[i].row == kcell.row)
        //         {
        //             cells[i].col--;
        //         }

        //         if (kcell.col == 0 && knext.row != kcell.row)
        //         {
        //             cells[i].row--;
        //         }
        //     }
        //     bufferSize--;
        // }
    }

    /* Handling Return */
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        Uint k = cursorIndex++;
        bufferSize++;

        cells[k].row = row++;
        cells[k].col = col;
        cells[k].data = '\n';

        col = 0;
    }
}

/*
 * Handling text input
 */
static void
glfwCharCallback(GLFWwindow *window, Uint codepoint)
{
    if (bufferSize >= maxbufferSize)
    {
        maxbufferSize = maxbufferSize + 100;

        cells = (Cell *)realloc(cells, maxbufferSize * sizeof(Cell));
        glyphs = (Glyph *)realloc(glyphs, maxbufferSize * sizeof(Glyph));
    }

    Uint k = cursorIndex++;

    cells[k].row = row;
    cells[k].col = col++;
    cells[k].data = codepoint;

    if (cursorIndex > bufferSize)
    {
        bufferSize++;
    }
}

/*
 * Resize callback
 */
void glfwResizeCallback(GLFWwindow *window, int newWidth, int newHeight)
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
    Char topbarText[topbarWidth];
    sprintf(topbarText, "File: %s, Size: %d/%d bytes, (%d,%d), %d",
            filePath,
            bufferSize,
            maxbufferSize,
            col,
            row,
            cursorIndex);

    glyphBufferDraw(topbar, topbarText, strlen(topbarText), &fontfamily, topbaroffset, colors[1]);

    cursor.pos.x = col * (fontfamily.faces[cursor.ch].advance >> 6) + offset.x;
    cursor.pos.y = row * fontfamily.height + offset.y;

    glyphDraw(&cursor, &fontfamily);

    for (size_t i = 0; i < bufferSize; i++)
    {
        if (glyphs[i].id == 0)
            glyphInit(&glyphs[i]);

        glyphs[i].pos.x = cells[i].col * (fontfamily.faces[cells[i].data].advance >> 6) + offset.x;
        glyphs[i].pos.y = cells[i].row * fontfamily.height + offset.y;

        glyphs[i].ch = cells[i].data;
        glyphs[i].color = colors[0];

        glyphDraw(&glyphs[i], &fontfamily);
    }
}

/*
 * Initializing App and allocating memory 
 */
void appInit(App *app, int argc, char *argv[])
{
    FILE *file = fopen(filePath, "r");

    /* Allocating memory */
    topbar = (Glyph *)malloc(topbarWidth * sizeof(Glyph));
    cells = (Cell *)malloc(maxbufferSize * sizeof(Cell));
    glyphs = (Glyph *)malloc(maxbufferSize * sizeof(Glyph));

    logInit(logPath);
    windowInit(&app->window, width, height);
    fontsInit(&fontfamily, fontPath);
    shaderInit(&shader);
    glyphInit(&cursor);

    // For topbar
    for (size_t i = 0; i < topbarWidth; i++)
    {
        glyphInit(&topbar[i]);
    }

    if (file)
    {
        Char *buffer;

        /* Find a better way to get size of a file */
        fseek(file, 0, SEEK_END);
        long fsize = ftell(file);
        fseek(file, 0, SEEK_SET);

        buffer = (Char *)malloc(fsize + 1);
        fread(buffer, sizeof(char), fsize, file);
        fclose(file);

        bufferSize = fsize;
        cursorIndex = fsize;
        if (bufferSize > maxbufferSize)
        {
            maxbufferSize = bufferSize + 100;
            cells = (Cell *)realloc(cells, maxbufferSize * sizeof(Cell));
            glyphs = (Glyph *)realloc(glyphs, maxbufferSize * sizeof(Glyph));
        }

        for (size_t i = 0; i < bufferSize; i++)
        {
            cells[i].row = row;
            cells[i].col = col++;
            cells[i].data = buffer[i];

            if (col > maxbufferSize || buffer[i] == '\n' || buffer[i] == '\r')
            {
                col = 0;
                row++;
            }

            glyphInit(&glyphs[i]);
        }
        free(buffer);
    }

    /* Registering callbacks */
    glfwSetKeyCallback(app->window.glfwWindow, glfwKeyCallback);
    glfwSetCharCallback(app->window.glfwWindow, glfwCharCallback);
    glfwSetFramebufferSizeCallback(app->window.glfwWindow, glfwResizeCallback);
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
        Char *buffer;
        buffer = (Char *)malloc(bufferSize);

        for (size_t i = 0; i < bufferSize; i++)
        {
            buffer[i] = cells[i].data;
        }

        fwrite(buffer, 1, bufferSize, file);
        fclose(file);
        logInfo("Buffer wrote to file");

        free(buffer);
        free(cells);
    }
    else
    {
        logError("Cannot write buffer to file");
    }

    logInfo("Resource cleanup done");
    logClose();
}
