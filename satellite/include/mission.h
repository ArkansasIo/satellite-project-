#ifndef ASTERIA_MISSION_H
#define ASTERIA_MISSION_H

#include <stdint.h>

#define ASTERIA_NAME "Asteria-1"
#define ASTERIA_CALLSIGN "AST1"
#define ASTERIA_SOFTWARE_VERSION "0.1.0"

typedef enum {
    MODE_BOOT = 0,
    MODE_SAFE = 1,
    MODE_NOMINAL = 2,
    MODE_SCIENCE = 3,
    MODE_DOWNLINK = 4
} MissionMode;

typedef struct {
    uint32_t tick;
    MissionMode mode;
    uint8_t battery_percent;
    int16_t bus_temp_c;
    uint8_t solar_input_percent;
    uint8_t attitude_lock;
    uint8_t payload_active;
    uint8_t downlink_active;
    uint32_t stored_packets;
    uint32_t rejected_commands;
} MissionState;

const char *mission_mode_name(MissionMode mode);
void mission_init(MissionState *state);
void mission_step(MissionState *state);
int mission_is_safe_to_operate_payload(const MissionState *state);

#endif
