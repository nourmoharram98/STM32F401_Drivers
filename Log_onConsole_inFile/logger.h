#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

// Enumeration for log levels
typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

// Function to initialize the logger
void logger_init(const char *filename);

// Function to log messages
void logger_log(LogLevel level, const char *format, ...);

// Function to close the logger
void logger_close();

#endif /* LOGGER_H */
