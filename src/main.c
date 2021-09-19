/*
 * Author : ayushkumar
 * This is the main entrypoint of the hedit
 */

#include <app.h>

/*
 * Driver code
 */
int main(int argc, char *argv[])
{
    App app = {.window = {0}};

    appInit(&app, argc, argv);
    appRun(&app);
    appCleanup();

    return 0;
}