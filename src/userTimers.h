#pragma once
#include <Arduino.h>


static uint8_t rgb_color = 0;        // Значение оттенка (0–255)
static uint16_t RED_PWM, GREEN_PWM, BLUE_PWM;         // Значения компонентов Red, Green, Blue (0–4095)
static uint16_t brightness = 4095; // Максимальная яркость (4095 для 12-битного PWM)



void hueToRGB(uint8_t hue, uint16_t brightness);
void init_pins();
void userTimers();


