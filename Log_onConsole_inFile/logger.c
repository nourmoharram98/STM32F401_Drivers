#include "logger.h"

static FILE *log_file = NULL;

void logger_init(const char *filename) {
    log_file = fopen(filename, "a");
    if (log_file == NULL) {
        fprintf(stderr, "Error opening log file.\n");
    }
}

void logger_log(LogLevel level, const char *format, ...) {
    va_list args;
    va_start(args, format);

    switch (level) {
        case LOG_INFO:
            printf("[INFO]: ");
            vprintf(format, args);
            break;
        case LOG_WARNING:
            printf("[WARNING]: ");
            vprintf(format, args);
            break;
        case LOG_ERROR:
            fprintf(stderr, "[ERROR]: ");
            vfprintf(stderr, format, args);
            break;
        default:
            break;
    }

    if (log_file != NULL) {
        vfprintf(log_file, format, args);
    }

    va_end(args);
}

void logger_close() {
    if (log_file != NULL) {
        fclose(log_file);
    }
}
