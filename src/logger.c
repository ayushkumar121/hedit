#include <logger.h>

FILE *logs;

/*
 * Initializing log file
 */
void logInit(Char *logFile)
{
    logs = fopen(logFile, "w");

    if (logs)
    {
        logInfo("Log file initialized");
    }
    else
    {
        logError("Cannot initilize log file using stdout instead");
        logs = stdout;
    }
}

/*
 * Log an information message
 */
void logInfo(const Char *fmt, ...)
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
void logWarn(const Char *fmt, ...)
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
void logError(const Char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(logs, "[ ERROR ] ");
    vfprintf(logs, fmt, args);
    fprintf(logs, "\n");
    fflush(logs);

    // fprintf(stderr, RED "[ ERROR ] " RESET);
    // vfprintf(stderr, fmt, args);
    // fprintf(stderr, "\n");
}

/*
 * Closing the log file
 */
void logClose()
{
    if (logs != stdout)
    {
        fclose(logs);
    }
}