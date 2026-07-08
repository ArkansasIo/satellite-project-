#include <stdio.h>
#include <string.h>

#include "commands.h"
#include "event_log.h"
#include "mission.h"
#include "platform.h"
#include "telemetry.h"

static char *trim_line(char *line) {
    char *start = line;
    while (*start == ' ' || *start == '\t' || *start == '\r' || *start == '\n') {
        start++;
    }

    char *end = start + strlen(start);
    while (end > start) {
        char previous = *(end - 1);
        if (previous != ' ' && previous != '\t' && previous != '\r' && previous != '\n') {
            break;
        }
        end--;
    }
    *end = '\0';
    return start;
}

static int run_command_file(MissionState *state, const char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open command file: %s\n", path);
        return 1;
    }

    char line[ASTERIA_MAX_COMMAND_LINE];
    while (fgets(line, sizeof(line), file) != NULL) {
        char *command_text = trim_line(line);
        if (command_text[0] == '\0' || command_text[0] == '#') {
            continue;
        }

        CommandId command = command_parse(command_text);
        printf("CMD %-12s -> %s\n", command_text, command_name(command));
        if (command == COMMAND_NONE) {
            event_log_write(state->tick, EVENT_WARN, "command", "ignored unknown command");
        } else {
            event_log_write(state->tick, EVENT_INFO, "command", command_name(command));
        }
        command_apply(state, command);
        mission_step(state);
        telemetry_print_frame(state);
        telemetry_print_health(state);
    }

    fclose(file);
    return 0;
}

int main(int argc, char **argv) {
    const char *command_file = argc > 1 ? argv[1] : ASTERIA_DEFAULT_COMMAND_FILE;
    MissionState state;
    mission_init(&state);
    event_log_reset();
    event_log_write(state.tick, EVENT_INFO, "boot", "simulator initialized");
    telemetry_print_banner();

    if (run_command_file(&state, command_file) != 0) {
        return 1;
    }

    for (unsigned int i = 0; i < 8; ++i) {
        mission_step(&state);
        telemetry_print_frame(&state);
        telemetry_print_health(&state);
    }

    event_log_write(state.tick, EVENT_INFO, "sim", "simulation complete");
    event_log_dump();
    puts("Simulation complete.");
    return 0;
}
