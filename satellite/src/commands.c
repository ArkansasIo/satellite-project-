#include "commands.h"

#include <ctype.h>
#include <string.h>

static int equals_ignore_case(const char *left, const char *right) {
    while (*left != '\0' && *right != '\0') {
        if (tolower((unsigned char)*left) != tolower((unsigned char)*right)) {
            return 0;
        }
        left++;
        right++;
    }
    return *left == '\0' && *right == '\0';
}

CommandId command_parse(const char *text) {
    if (text == 0) {
        return COMMAND_NONE;
    }
    if (equals_ignore_case(text, "safe")) {
        return COMMAND_SAFE_MODE;
    }
    if (equals_ignore_case(text, "nominal")) {
        return COMMAND_NOMINAL_MODE;
    }
    if (equals_ignore_case(text, "science")) {
        return COMMAND_START_SCIENCE;
    }
    if (equals_ignore_case(text, "downlink")) {
        return COMMAND_START_DOWNLINK;
    }
    if (equals_ignore_case(text, "stop_payload")) {
        return COMMAND_STOP_PAYLOAD;
    }
    return COMMAND_NONE;
}

const char *command_name(CommandId command) {
    switch (command) {
        case COMMAND_SAFE_MODE:
            return "SAFE";
        case COMMAND_NOMINAL_MODE:
            return "NOMINAL";
        case COMMAND_START_SCIENCE:
            return "SCIENCE";
        case COMMAND_START_DOWNLINK:
            return "DOWNLINK";
        case COMMAND_STOP_PAYLOAD:
            return "STOP_PAYLOAD";
        case COMMAND_NONE:
        default:
            return "NONE";
    }
}

void command_apply(MissionState *state, CommandId command) {
    switch (command) {
        case COMMAND_SAFE_MODE:
            state->mode = MODE_SAFE;
            state->payload_active = 0;
            state->downlink_active = 0;
            break;
        case COMMAND_NOMINAL_MODE:
            state->mode = MODE_NOMINAL;
            break;
        case COMMAND_START_SCIENCE:
            if (!mission_is_safe_to_operate_payload(state)) {
                state->rejected_commands++;
                break;
            }
            state->mode = MODE_SCIENCE;
            state->payload_active = 1;
            break;
        case COMMAND_START_DOWNLINK:
            if (state->battery_percent < 30) {
                state->rejected_commands++;
                break;
            }
            state->mode = MODE_DOWNLINK;
            state->downlink_active = 1;
            break;
        case COMMAND_STOP_PAYLOAD:
            state->payload_active = 0;
            if (state->mode == MODE_SCIENCE) {
                state->mode = MODE_NOMINAL;
            }
            break;
        case COMMAND_NONE:
        default:
            break;
    }
}
