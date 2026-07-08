#include <stdio.h>

#include "commands.h"
#include "health.h"
#include "mission.h"
#include "storage.h"

static int failures = 0;

static void check_int(const char *name, int expected, int actual) {
    if (expected != actual) {
        printf("FAIL %s: expected %d actual %d\n", name, expected, actual);
        failures++;
    }
}

static void test_mode_transitions(void) {
    MissionState state;
    mission_init(&state);

    command_apply(&state, COMMAND_NOMINAL_MODE);
    check_int("nominal mode", MODE_NOMINAL, state.mode);

    command_apply(&state, COMMAND_START_SCIENCE);
    check_int("science mode", MODE_SCIENCE, state.mode);
    check_int("payload active", 1, state.payload_active);

    command_apply(&state, COMMAND_STOP_PAYLOAD);
    check_int("payload stopped", 0, state.payload_active);
    check_int("mode after stop payload", MODE_NOMINAL, state.mode);

    command_apply(&state, COMMAND_SAFE_MODE);
    check_int("safe mode", MODE_SAFE, state.mode);
    check_int("payload disabled in safe", 0, state.payload_active);
    check_int("downlink disabled in safe", 0, state.downlink_active);
}

static void test_safety_rejects_payload(void) {
    MissionState state;
    mission_init(&state);
    state.battery_percent = 20;
    command_apply(&state, COMMAND_START_SCIENCE);

    check_int("unsafe payload rejected", 1, (int)state.rejected_commands);
    check_int("payload remains off", 0, state.payload_active);
}

static void test_payload_and_downlink_packets(void) {
    MissionState state;
    mission_init(&state);
    command_apply(&state, COMMAND_NOMINAL_MODE);
    command_apply(&state, COMMAND_START_SCIENCE);
    mission_step(&state);

    check_int("science stores packets", 3, (int)state.stored_packets);

    command_apply(&state, COMMAND_START_DOWNLINK);
    mission_step(&state);
    check_int("downlink drains net packets", 4, (int)state.stored_packets);
}

static void test_health_and_storage(void) {
    MissionState state;
    mission_init(&state);
    state.battery_percent = 20;

    HealthReport report = health_evaluate(&state);
    check_int("critical battery health", HEALTH_CRITICAL, report.power);
    check_int("critical overall health", HEALTH_CRITICAL, report.overall);

    StorageState storage;
    storage_init(&storage, 5);
    check_int("storage accepts capacity", 5, (int)storage_write_packets(&storage, 7));
    check_int("storage drops overflow", 2, (int)storage.dropped_packets);
    check_int("storage read", 3, (int)storage_read_packets(&storage, 3));
    check_int("storage remaining", 2, (int)storage.used_packets);
}

int main(void) {
    test_mode_transitions();
    test_safety_rejects_payload();
    test_payload_and_downlink_packets();
    test_health_and_storage();

    if (failures != 0) {
        printf("%d test failure(s)\n", failures);
        return 1;
    }

    puts("All mission tests passed.");
    return 0;
}
