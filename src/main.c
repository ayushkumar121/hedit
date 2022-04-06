/*
 * Author : ayushkumar
 * This is the main entrypoint of the hedit
 */

#include <app.h>

int main(int argc, char *argv[])
{
    App app = {.window = {0}};

    AppInit(&app, argc, argv);
    AppRun(&app);
    AppCleanup(&app);

    return 0;
}