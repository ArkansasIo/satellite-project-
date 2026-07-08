#include "scheduler.h"

void scheduler_init(ScheduleEntry *entries, unsigned int count) {
    for (unsigned int i = 0; i < count; ++i) {
        entries[i].last_run_tick = 0;
    }
}

void scheduler_run_due(ScheduleEntry *entries, unsigned int count, MissionState *state) {
    for (unsigned int i = 0; i < count; ++i) {
        ScheduleEntry *entry = &entries[i];
        if (entry->task == 0 || entry->period_ticks == 0) {
            continue;
        }
        if (state->tick == 0 || state->tick - entry->last_run_tick >= entry->period_ticks) {
            entry->task(state);
            entry->last_run_tick = state->tick;
        }
    }
}
