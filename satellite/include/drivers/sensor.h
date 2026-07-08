#ifndef ASTERIA_DRIVERS_SENSOR_H
#define ASTERIA_DRIVERS_SENSOR_H

#include <stdint.h>

typedef struct {
    uint16_t radiation_counts;
    int16_t camera_temp_c;
    uint8_t image_ready;
} PayloadSample;

PayloadSample sensor_capture_simulated(uint32_t tick);

#endif
