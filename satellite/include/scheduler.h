#ifndef ASTERIA_SCHEDULER_H
#define ASTERIA_SCHEDULER_H

#include "mission.h"

typedef void (*ScheduledTask)(MissionState *state);

typedef struct {
    const char *name;
    uint32_t period_ticks;
    uint32_t last_run_tick;
    ScheduledTask task;
} ScheduleEntry;

void scheduler_init(ScheduleEntry *entries, unsigned int count);
void scheduler_run_due(ScheduleEntry *entries, unsigned int count, MissionState *state);

#endif
