#ifndef ASTERIA_COMMANDS_H
#define ASTERIA_COMMANDS_H

#include "mission.h"

typedef enum {
    COMMAND_NONE = 0,
    COMMAND_SAFE_MODE,
    COMMAND_NOMINAL_MODE,
    COMMAND_START_SCIENCE,
    COMMAND_START_DOWNLINK,
    COMMAND_STOP_PAYLOAD
} CommandId;

CommandId command_parse(const char *text);
const char *command_name(CommandId command);
void command_apply(MissionState *state, CommandId command);

#endif
