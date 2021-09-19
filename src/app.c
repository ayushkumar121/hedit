#include <app.h>

/* Global variables */
Char *filePath = "buffer.txt";
Char *logPath = "hedit.logs";
Char *fontPath = "assets/fonts/Cascadia.ttf";

Uint cols = 80;
Uint rows = 40;

Uint width = 640;
Uint height = 480;

Glyph cursor = {.ch = '_', .color = {.r = 0.0f, .g = 1.0f, .b = 0.0f, .a = 1.0f}};
FontFamily fontfamily = {.height = 16};
Shader shader = {0};

/* For current buffer */
Int col = 0;
Int row = 0;
Uint bufferSize = 0;

Char *buffer;
Cell *cells;
Glyph *glyphs;
V2 offset = {.x = 10.0f, .y = 40.0f};
V4 colors[] =
    {
        /* foreground color */
        [0] = {.r = 0.92f, .g = 0.86f, .b = 0.70f, .a = 1.0f},
        [1] = {.r = 0.83f, .g = 0.36f, .b = 0.05f, .a = 1.0f},
        [2] = {.r = 0.0f, .g = 1.0f, .b = 0.0f, .a = 1.0f}};

/* Topbar */
Glyph *topbar;
V2 topbaroffset = {.x = 10.0f, .y = 20.0f};

/* Opening file buffers */
FILE *file;

/*
 * GLFW Callbacks 
 */

/*
 * Handling individual key presses like backspaces and return
 */
static void
glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (bufferSize < rows * cols)
    {
        if (bufferSize > 0 && key == GLFW_KEY_BACKSPACE)
        {
            bufferSize--;

            if (col <= 0 && row > 1)
            {
                col = cells[bufferSize].col;
                row--;
            }
            else
            {
                col--;
            }
        }

        if (key == GLFW_KEY_ENTER)
        {
            Uint i = bufferSize++;

            cells[i].row = row;
            cells[i].col = col;
            cells[i].data = '\n';

            col = 0;
            row++;

            glyphInit(&glyphs[i]);
        }
    }
}

/*
 * Handling text input
 */
static void
glfwCharCallback(GLFWwindow *window, Uint codepoint)
{
    if (bufferSize < rows * cols)
    {
        Uint i = bufferSize++;

        cells[i].row = row;
        cells[i].col = col;
        cells[i].data = codepoint;

        col++;

        glyphInit(&glyphs[i]);
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
    Char topbarText[40];
    sprintf(topbarText, "File: %s, Size: %d bytes", filePath, bufferSize);

    glyphBufferDraw(topbar, topbarText, strlen(topbarText), &fontfamily, topbaroffset, colors[1]);

    for (size_t i = 0; i < bufferSize; i++)
    {
        glyphs[i].pos.x = cells[i].col * (fontfamily.faces[cells[i].data].advance >> 6) + offset.x;
        glyphs[i].pos.y = cells[i].row * fontfamily.height + offset.y;

        glyphs[i].ch = cells[i].data;
        glyphs[i].color = colors[0];

        glyphDraw(&glyphs[i], &fontfamily);
    }

    cursor.pos.x = col * (fontfamily.faces[cursor.ch].advance >> 6) + offset.x;
    cursor.pos.y = row * fontfamily.height + offset.y;
}

/*
 * Initializing App and allocating memory 
 */
void appInit(App *app, int argc, char *argv[])
{
    file = fopen(filePath, "r");

    /* Allocating memory */
    cells = (Cell *)malloc(rows * cols * sizeof(Cell));
    buffer = (Char *)malloc(rows * cols * sizeof(Char));
    glyphs = (Glyph *)malloc(rows * cols * sizeof(Glyph));
    topbar = (Glyph *)malloc(cols * sizeof(Glyph));

    logInit(logPath);
    windowInit(&app->window, width, height);
    fontsInit(&fontfamily, fontPath);
    shaderInit(&shader);
    glyphInit(&cursor);

    if (file)
    {
        /* Find a better way to get size of a file */
        fseek(file, 0, SEEK_END);
        long fsize = ftell(file);
        fseek(file, 0, SEEK_SET);

        fread(buffer, sizeof(char), fsize, file);
        fclose(file);

        bufferSize = fsize;
        for (size_t i = 0; i < bufferSize; i++)
        {
            cells[i].row = row;
            cells[i].col = col;
            cells[i].data = buffer[i];

            col++;

            if (col > cols || buffer[i] == '\n' || buffer[i] == '\r')
            {
                col = 0;
                row++;
            }
        }
    }
    file = fopen(filePath, "w+");

    for (size_t i = 0; i < bufferSize; i++)
    {
        glyphInit(&glyphs[i]);
    }

    for (size_t i = 0; i < cols; i++)
    {
        glyphInit(&topbar[i]);
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
        windowBind(width, height);

        appLoop(app);

        glyphDraw(&cursor, &fontfamily);
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
    for (size_t i = 0; i < rows * cols; i++)
    {
        glyphCleanup(&glyphs[i]);
    }

    for (size_t i = 0; i < cols; i++)
    {
        glyphCleanup(&topbar[i]);
    }

    fontsCleanup(&fontfamily);
    shadersCleanup(&shader);
    windowCleanup();

    if (file)
    {
        for (size_t i = 0; i < bufferSize; i++)
        {
            buffer[i] = cells[i].data;
        }

        fwrite(buffer, 1, bufferSize, file);
        fclose(file);
        logInfo("Buffer wrote to file");
    }
    else
    {
        logError("Cannot write buffer to file");
    }

    // free(cells);
    // free(glyphs);
    // free(topbar);
    // free(buffer);

    logInfo("Resource cleanup done");
    logClose();
}
