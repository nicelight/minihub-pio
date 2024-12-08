#pragma once
#include <Arduino.h>
#include <GyverDS18.h>
#include "nastroyki.h"


// тут мы говорим что есть такие объекты и они доступны для других функций,
//  а сами объекты определяем уже в userTimers.cpp файле 
extern GyverDS18Single ds1;  // пин PIN_DS18B20_1
extern GyverDS18Single ds2;  // пин PIN_DS18B20_2


void hueToRGB(uint8_t hue, uint16_t brightness);
void init_pins();
void read_t1_from_db();
void userNatureTimer();
void userSixTimers();

void getds18();

