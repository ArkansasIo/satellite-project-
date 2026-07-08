#ifndef ASTERIA_DRIVERS_ADC_H
#define ASTERIA_DRIVERS_ADC_H

#include <stdint.h>

typedef struct {
    uint16_t battery_mv;
    int16_t board_temp_c;
    uint16_t solar_panel_mv;
} AdcSample;

AdcSample adc_read_simulated(uint32_t tick);

#endif
