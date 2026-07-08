#include "drivers/radio.h"

void radio_init(RadioState *radio) {
    radio->link_ready = 1;
    radio->frames_sent = 0;
}

uint8_t radio_send_frame(RadioState *radio, const char *frame) {
    if (radio->link_ready == 0 || frame == 0 || frame[0] == '\0') {
        return 0;
    }
    radio->frames_sent++;
    return 1;
}
