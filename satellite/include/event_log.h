#ifndef ASTERIA_EVENT_LOG_H
#define ASTERIA_EVENT_LOG_H

#include <stdint.h>

typedef enum {
    EVENT_INFO = 0,
    EVENT_WARN = 1,
    EVENT_ERROR = 2
} EventSeverity;

void event_log_reset(void);
void event_log_write(uint32_t tick, EventSeverity severity, const char *source, const char *message);
void event_log_dump(void);

#endif
