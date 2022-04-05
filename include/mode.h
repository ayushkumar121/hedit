#ifndef MODES_H
#define MODES_H

#include <gmath.h>

typedef enum Mode
{
    COMMAND,
    NORMAL
} Mode;

Char* ModeString(Mode mode);

#endif
