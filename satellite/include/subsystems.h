#ifndef ASTERIA_SUBSYSTEMS_H
#define ASTERIA_SUBSYSTEMS_H

#include "mission.h"

void power_update(MissionState *state);
void thermal_update(MissionState *state);
void attitude_update(MissionState *state);
void payload_update(MissionState *state);
void comms_update(MissionState *state);

#endif
