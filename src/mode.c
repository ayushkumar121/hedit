#include <mode.h>

Char *ModeString(Mode mode)
{
    switch (mode)
    {
    case COMMAND:
        return "COMMAND";
    case NORMAL:
        return "NORMAL";
    }

    return "UNKNOWN";
}