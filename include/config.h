#ifndef CONFIG_H
#define CONFIG_H

#include <gmath.h>

#define FontPath "assets/fonts/Iosevka Term.ttf"
#define FontHeight 16
#define FontLineSpace 5

const static Vec2 topbarOffset = {10.0f, 40.0f};
const static Vec2 bufferOffset = {10.0f, 70.0f};

enum Color {
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    WHITE,
    BLACK,
    FOREGROUND,
    BACKGROUND,
};

const static Vec4 colors[] = {
    [RED] = {1.0f, 0.0f, 0.0f, 1.0f},
    [GREEN] = {0.0f, 1.0f, 0.0f, 1.0f},
    [YELLOW] = {1.0f, 1.0f, 0.0f, 1.0f},
    [BLUE] = {1.0f, 0.0f, 1.0f, 1.0f},
    [MAGENTA] = {0.0f, 0.5f, 1.0f, 1.0f},
    [WHITE] = {1.0f, 1.0f, 1.0f, 1.0f},
    [BLACK] = {0.0f, 0.0f, 0.0f, 1.0f},
    [FOREGROUND] = {1.0f, 1.0f, 1.0f, 1.0f},
    [BACKGROUND] = {0.0f, 0.0f, 0.0f, 1.0f},
};

#endif /* CONFIG_H */