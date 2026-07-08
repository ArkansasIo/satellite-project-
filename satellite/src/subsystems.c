#include "subsystems.h"

void power_update(MissionState *state) {
    if (state->solar_input_percent > 55 && state->battery_percent < 100) {
        state->battery_percent++;
    }
    if ((state->payload_active || state->downlink_active) && state->battery_percent > 0) {
        state->battery_percent--;
    }
    if (state->battery_percent < 25) {
        state->mode = MODE_SAFE;
        state->payload_active = 0;
        state->downlink_active = 0;
    }
}

void thermal_update(MissionState *state) {
    if (state->payload_active || state->downlink_active) {
        state->bus_temp_c++;
    } else if (state->bus_temp_c > 12) {
        state->bus_temp_c--;
    }
    if (state->bus_temp_c > 45) {
        state->mode = MODE_SAFE;
        state->payload_active = 0;
        state->downlink_active = 0;
    }
}

void attitude_update(MissionState *state) {
    state->attitude_lock = state->mode != MODE_BOOT;
}

void payload_update(MissionState *state) {
    if (state->payload_active) {
        state->stored_packets += 3;
    }
}

void comms_update(MissionState *state) {
    if (state->downlink_active && state->stored_packets > 0) {
        if (state->stored_packets >= 2) {
            state->stored_packets -= 2;
        } else {
            state->stored_packets = 0;
        }
    }
}
