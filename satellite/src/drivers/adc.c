#include "drivers/adc.h"

AdcSample adc_read_simulated(uint32_t tick) {
    AdcSample sample;
    sample.battery_mv = (uint16_t)(7600U + (tick % 40U));
    sample.board_temp_c = (int16_t)(18 + (int16_t)(tick % 6U));
    sample.solar_panel_mv = (uint16_t)(5200U + ((tick * 3U) % 200U));
    return sample;
}
