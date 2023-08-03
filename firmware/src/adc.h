#ifndef YARR_ADC_H
#define YARR_ADC_H

#include <Arduino.h>
#include <MCP3208.h>

#include <Wire.h>
#include <MCP3X21.h>

extern float busVoltage;

void adc_setup();
void adc_loop();

uint16_t adc_readMCP3208Channel(byte channel, byte samples = 64);
float adc_readAmperage(byte channel);

float adc_readBusVoltage();
int adc_readBusVoltageADC();

#endif /* !YARR_ADC_H */