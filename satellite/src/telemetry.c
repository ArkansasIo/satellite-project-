#include "telemetry.h"

#include "health.h"

#include <stdio.h>

void telemetry_print_banner(void) {
    puts("ASTERIA-1 NEW SATELLITE FLIGHT SOFTWARE");
    puts("----------------------------------------");
    puts("Educational simulation only. No real spacecraft control.");
    puts("");
}

void telemetry_print_frame(const MissionState *state) {
    printf(
        "T+%04u mode=%-8s batt=%3u%% temp=%3dC solar=%3u%% att=%s payload=%s downlink=%s packets=%u rejected=%u\n",
        (unsigned int)state->tick,
        mission_mode_name(state->mode),
        (unsigned int)state->battery_percent,
        (int)state->bus_temp_c,
        (unsigned int)state->solar_input_percent,
        state->attitude_lock ? "lock" : "none",
        state->payload_active ? "on " : "off",
        state->downlink_active ? "on " : "off",
        (unsigned int)state->stored_packets,
        (unsigned int)state->rejected_commands
    );
}

void telemetry_print_health(const MissionState *state) {
    HealthReport report = health_evaluate(state);
    printf(
        "HEALTH overall=%s power=%s thermal=%s attitude=%s storage=%s\n",
        health_status_name(report.overall),
        health_status_name(report.power),
        health_status_name(report.thermal),
        health_status_name(report.attitude),
        health_status_name(report.storage)
    );
}
