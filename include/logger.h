#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

#include <gmath.h>

void LogInit(const Char *logFile);
void LogInfo(const Char *fmt, ...);
void LogWarn(const Char *fmt, ...);
void LogError(const Char *fmt, ...);
void LogClose();

#endif /* LOGGER_H */