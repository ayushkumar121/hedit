#include <logger.h>

FILE *logs;

/*
 * Initializing log file
 */
void LogInit(const Char *logFile)
{
    logs = fopen(logFile, "w");

    if (logs)
    {
        LogInfo("Log file initialized");
    }
    else
    {
        LogError("Cannot initilize log file using stdout instead");
        logs = stdout;
    }
}

/*
 * Log an information message
 */
void LogInfo(const Char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(logs, "[ INFO ] ");
    vfprintf(logs, fmt, args);
    fprintf(logs, "\n");
    fflush(logs);
}

/*
 * Log an warning message
 */
void LogWarn(const Char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(logs, "[ WARN ] ");
    vfprintf(logs, fmt, args);
    fprintf(logs, "\n");
    fflush(logs);
}

/*
 * Log an error message
 */
void LogError(const Char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(logs, "[ ERROR ] ");
    vfprintf(logs, fmt, args);
    fprintf(logs, "\n");
    fflush(logs);
}

/*
 * Closing the log file
 */
void LogClose()
{
    if (logs != stdout)
    {
        fclose(logs);
    }
}