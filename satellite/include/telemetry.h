#ifndef ASTERIA_TELEMETRY_H
#define ASTERIA_TELEMETRY_H

#include "mission.h"

void telemetry_print_banner(void);
void telemetry_print_frame(const MissionState *state);
void telemetry_print_health(const MissionState *state);

#endif
