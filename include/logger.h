#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

#include <gmath.h>

#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
#define RESET  "\x1B[0m"

void logInit(Char *logFile);
void logInfo(const Char *fmt, ...);
void logWarn(const Char *fmt, ...);
void logError(const Char *fmt, ...);
void logClose();

#endif /* LOGGER_H */