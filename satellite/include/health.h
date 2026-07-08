#ifndef ASTERIA_HEALTH_H
#define ASTERIA_HEALTH_H

#include "mission.h"

typedef enum {
    HEALTH_OK = 0,
    HEALTH_WARN = 1,
    HEALTH_CRITICAL = 2
} HealthStatus;

typedef struct {
    HealthStatus power;
    HealthStatus thermal;
    HealthStatus attitude;
    HealthStatus storage;
    HealthStatus overall;
} HealthReport;

HealthReport health_evaluate(const MissionState *state);
const char *health_status_name(HealthStatus status);

#endif
