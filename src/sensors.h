#pragma once
#include <Arduino.h>

#include <GyverDS18.h>
#include <DHT.h>

extern GyverDS18Single ds1;  // пин PIN_DS18B20_1
extern GyverDS18Single ds2;  // пин PIN_DS18B20_2

extern DHT dht1;

void getds18();
void getdht();
