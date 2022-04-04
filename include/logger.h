#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

#include <gmath.h>

void logInit(const Char *logFile);
void logInfo(const Char *fmt, ...);
void logWarn(const Char *fmt, ...);
void logError(const Char *fmt, ...);
void logClose();

#endif /* LOGGER_H */