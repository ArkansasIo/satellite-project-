#include "event_log.h"

#include <stdio.h>

#define EVENT_LOG_CAPACITY 32
#define EVENT_TEXT_LIMIT 80

typedef struct {
    uint32_t tick;
    EventSeverity severity;
    char source[16];
    char message[EVENT_TEXT_LIMIT];
} EventRecord;

static EventRecord records[EVENT_LOG_CAPACITY];
static unsigned int record_count = 0;
static unsigned int next_record = 0;

static const char *severity_name(EventSeverity severity) {
    switch (severity) {
        case EVENT_INFO:
            return "INFO";
        case EVENT_WARN:
            return "WARN";
        case EVENT_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

static void copy_text(char *dest, unsigned int dest_size, const char *src) {
    unsigned int i = 0;
    if (dest_size == 0) {
        return;
    }
    while (src != 0 && src[i] != '\0' && i + 1 < dest_size) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void event_log_reset(void) {
    record_count = 0;
    next_record = 0;
}

void event_log_write(uint32_t tick, EventSeverity severity, const char *source, const char *message) {
    EventRecord *record = &records[next_record];
    record->tick = tick;
    record->severity = severity;
    copy_text(record->source, sizeof(record->source), source);
    copy_text(record->message, sizeof(record->message), message);

    next_record = (next_record + 1U) % EVENT_LOG_CAPACITY;
    if (record_count < EVENT_LOG_CAPACITY) {
        record_count++;
    }
}

void event_log_dump(void) {
    puts("");
    puts("EVENT LOG");
    puts("---------");

    unsigned int start = record_count == EVENT_LOG_CAPACITY ? next_record : 0;
    for (unsigned int i = 0; i < record_count; ++i) {
        unsigned int index = (start + i) % EVENT_LOG_CAPACITY;
        const EventRecord *record = &records[index];
        printf(
            "T+%04u %-5s %-12s %s\n",
            (unsigned int)record->tick,
            severity_name(record->severity),
            record->source,
            record->message
        );
    }
}
