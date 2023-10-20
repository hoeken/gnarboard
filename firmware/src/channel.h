/*
  Yarrboard
  
  Author: Zach Hoeken <hoeken@gmail.com>
  Website: https://github.com/hoeken/yarrboard
  License: GPLv3
*/

#ifndef YARR_CHANNEL_H
#define YARR_CHANNEL_H

#include <Arduino.h>
#include "prefs.h"
#include "config.h"
#include "driver/ledc.h"
#include "adchelper.h"
#include "protocol.h"

class OutputChannel
{
  byte _pins[CHANNEL_COUNT] = CHANNEL_PINS;

  public:
    byte id = 0;
    bool state = false;
    bool tripped = false;
    char name[YB_CHANNEL_NAME_LENGTH];

    unsigned int stateChangeCount = 0;
    unsigned int softFuseTripCount = 0;

    float dutyCycle = 0.0;
    float lastDutyCycle = 0.0;
    unsigned long lastDutyCycleUpdate = 0;
    unsigned long dutyCycleIsThrottled = 0;

    bool fadeRequested = false;
    unsigned long fadeStartTime = 0;
    unsigned long fadeEndTime = 0;
    float fadeDutyCycleStart = 0;
    float fadeDutyCycleEnd = 0;

    MCP3208Helper *adcHelper;
    float amperage = 0.0;
    float softFuseAmperage = 0.0;
    float ampHours = 0.0;
    float wattHours = 0.0;

    bool isEnabled = false;
    bool isDimmable = false;

    void setup();
    void setupLedc();
    void setupInterrupt();
    void saveThrottledDutyCycle();
    void updateOutput();
    float getAmperage();
    float toAmperage(float voltage);
    void checkAmperage();
    void checkSoftFuse();
    void checkIfFadeOver();
    void setFade(float duty, int max_fade_time_ms);
    void setDuty(float duty);
    void calculateAverages(unsigned int delta);
};

extern OutputChannel channels[CHANNEL_COUNT];

void channel_setup();
void channel_loop();

#endif /* !YARR_CHANNEL_H */