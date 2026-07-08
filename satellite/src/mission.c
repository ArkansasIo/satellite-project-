#include "mission.h"

#include "subsystems.h"

const char *mission_mode_name(MissionMode mode) {
    switch (mode) {
        case MODE_BOOT:
            return "BOOT";
        case MODE_SAFE:
            return "SAFE";
        case MODE_NOMINAL:
            return "NOMINAL";
        case MODE_SCIENCE:
            return "SCIENCE";
        case MODE_DOWNLINK:
            return "DOWNLINK";
        default:
            return "UNKNOWN";
    }
}

void mission_init(MissionState *state) {
    state->tick = 0;
    state->mode = MODE_BOOT;
    state->battery_percent = 78;
    state->bus_temp_c = 18;
    state->solar_input_percent = 66;
    state->attitude_lock = 0;
    state->payload_active = 0;
    state->downlink_active = 0;
    state->stored_packets = 0;
    state->rejected_commands = 0;
}

void mission_step(MissionState *state) {
    state->tick++;
    power_update(state);
    thermal_update(state);
    attitude_update(state);
    payload_update(state);
    comms_update(state);
}

int mission_is_safe_to_operate_payload(const MissionState *state) {
    return state->battery_percent >= 35 && state->bus_temp_c <= 38 && state->mode != MODE_SAFE;
}
