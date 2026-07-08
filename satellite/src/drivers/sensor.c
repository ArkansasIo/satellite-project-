#include "drivers/sensor.h"

PayloadSample sensor_capture_simulated(uint32_t tick) {
    PayloadSample sample;
    sample.radiation_counts = (uint16_t)(100U + ((tick * 7U) % 50U));
    sample.camera_temp_c = (int16_t)(12 + (int16_t)(tick % 8U));
    sample.image_ready = tick % 3U == 0U;
    return sample;
}
