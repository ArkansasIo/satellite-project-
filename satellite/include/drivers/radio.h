#ifndef ASTERIA_DRIVERS_RADIO_H
#define ASTERIA_DRIVERS_RADIO_H

#include <stdint.h>

typedef struct {
    uint8_t link_ready;
    uint32_t frames_sent;
} RadioState;

void radio_init(RadioState *radio);
uint8_t radio_send_frame(RadioState *radio, const char *frame);

#endif
