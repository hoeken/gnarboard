#ifndef _CONFIG_H_8CH_MOSFET_REVB
#define _CONFIG_H_8CH_MOSFET_REVB

#define YB_HARDWARE_VERSION "RGB_INPUT_REV_B"

#define YB_HAS_INPUT_CHANNELS
#define YB_INPUT_CHANNEL_COUNT 8
#define YB_INPUT_CHANNEL_PINS { 26, 25, 33, 32, 35, 34, 39, 36 }
#define YB_INPUT_DEBOUNCE_RATE_MS 20

#define YB_HAS_ADC_CHANNELS
#define YB_ADC_CHANNEL_COUNT 8
#define YB_ADC_DRIVER_MCP3208
#define YB_ADC_RESOLUTION 12
#define YB_ADC_CS 17

#define YB_HAS_RGB_CHANNELS
#define YB_RGB_CHANNEL_COUNT 8
#define YB_RGB_CHANNEL_RESOLUTION 12
#define YB_RGB_UPDATE_RATE_MS 50
#define YB_RGB_DRIVER_TLC5947
#define YB_RGB_TLC5947_NUM 1
#define YB_RGB_TLC5947_CLK 14
#define YB_RGB_TLC5947_DATA 13
#define YB_RGB_TLC5947_LATCH 27
#define YB_RGB_TLC5947_BLANK 4

#endif // _CONFIG_H_8CH_MOSFET_REVB