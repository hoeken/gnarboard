/*
  Yarrboard
  
  Author: Zach Hoeken <hoeken@gmail.com>
  Website: https://github.com/hoeken/yarrboard
  License: GPLv3
*/

#ifndef YARR_CONFIG_H
#define YARR_CONFIG_H

#include <ArduinoTrace.h>

// enable one of these
//#define CONFIG_8CH_MOSFET_REVA
//#define CONFIG_8CH_MOSFET_REVB

#if defined CONFIG_8CH_MOSFET_REVA
  #include "./configs/config.8ch-mosfet-reva.h"
#elif defined CONFIG_8CH_MOSFET_REVB
  #include "./configs/config.8ch-mosfet-revb.h"
#endif

#ifndef CHANNEL_PWM_FREQUENCY
  #define CHANNEL_PWM_FREQUENCY 1000
#endif

#ifndef CHANNEL_PWM_RESOLUTION
  #define CHANNEL_PWM_RESOLUTION 10
#endif

#define YB_FAN_SINGLE_CHANNEL_AMPS 5.0
#define YB_FAN_AVERAGE_CHANNEL_AMPS 5.0
#define YB_FAN_MAX_CHANNEL_AMPS 20.0

//time before saving fade pwm to preserve flash
#define YB_DUTY_SAVE_TIMEOUT 5000

//bytes for sending json
#define MAX_JSON_LENGTH 1500

#define YBP_MODE_WEBSOCKET 0
#define YBP_MODE_HTTP      1
#define YBP_MODE_SERIAL    2

//for handling messages outside of the loop
#define YB_RECEIVE_BUFFER_LENGTH 512
#define YB_RECEIVE_BUFFER_COUNT 16

//milliseconds between sending updates on websocket and serial
#define YB_UPDATE_FREQUENCY 250
#define YB_ADC_INTERVAL 50
#define YB_ADC_SAMPLES 1

#define YB_PREF_KEY_LENGTH 16
#define YB_BOARD_NAME_LENGTH 31
#define YB_USERNAME_LENGTH 31
#define YB_PASSWORD_LENGTH 31
#define YB_CHANNEL_NAME_LENGTH 31
#define YB_WIFI_SSID_LENGTH 33
#define YB_WIFI_PASSWORD_LENGTH 64
#define YB_HOSTNAME_LENGTH 64

#endif // YARR_CONFIG_H