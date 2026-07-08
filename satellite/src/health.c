#include "health.h"

static HealthStatus worst(HealthStatus left, HealthStatus right) {
    return left > right ? left : right;
}

const char *health_status_name(HealthStatus status) {
    switch (status) {
        case HEALTH_OK:
            return "OK";
        case HEALTH_WARN:
            return "WARN";
        case HEALTH_CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
    }
}

HealthReport health_evaluate(const MissionState *state) {
    HealthReport report;

    report.power = HEALTH_OK;
    if (state->battery_percent < 35) {
        report.power = HEALTH_WARN;
    }
    if (state->battery_percent < 25) {
        report.power = HEALTH_CRITICAL;
    }

    report.thermal = HEALTH_OK;
    if (state->bus_temp_c > 38) {
        report.thermal = HEALTH_WARN;
    }
    if (state->bus_temp_c > 45) {
        report.thermal = HEALTH_CRITICAL;
    }

    report.attitude = state->attitude_lock ? HEALTH_OK : HEALTH_WARN;
    report.storage = state->stored_packets > 900 ? HEALTH_WARN : HEALTH_OK;

    report.overall = HEALTH_OK;
    report.overall = worst(report.overall, report.power);
    report.overall = worst(report.overall, report.thermal);
    report.overall = worst(report.overall, report.attitude);
    report.overall = worst(report.overall, report.storage);

    return report;
}
