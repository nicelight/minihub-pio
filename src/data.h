/* как пользоваться кошерно данными
читаем тут
 https://alexgyver.ru/docs/program-flow/cpp-files/#9-toc-title
пункт Составные типы

вот пример организации файлов
// data.h
#pragma once
struct Data {};
extern Data data;

// data.cpp
#include "data.h"
Data data;

//main.cpp
#include "data.h"
// переменная data уже видна тут
*/

#pragma once
#include <Arduino.h>

struct Data  // обьявляем класс структуры
{
    uint32_t secondsNow = 12;
    uint32_t secondsUptime = 1;
    // чтобы не проверять булевы значения ползунков в интерфейсе из базы, пишем их сюда сначала
    bool t1discr_enbl = 0; 
    bool rel1_on = 0;
    bool rel2_on = 0;
    bool rel3_on = 0;
    bool rel4_on = 0;
    bool rel5_on = 0;
    bool rel6_on = 0;
    int16_t temp_ds1 = 18;  // датчик температуры
    // природное освещение
    bool t1f_enbl = 0;  // ползунок переключатель в веб интерфесе()
    bool t1isWorks = 0; // на данный момент Природное светится
    uint32_t t1Sunrise_step = 1;  // шаг рассвета
    uint32_t t1f1_time = 21600;   // рассвет в 6 по дефолту
    uint32_t t1f2_time = 25200;   // утро начинается в 7
    uint32_t t1f3_time = 43200;   // обед в 12
    uint32_t t1f4_time = 64800;   // ужин в 18
    uint32_t t1f5_time = 72000;   // закат в  в 20
    uint32_t t1f6_time = 75600;   // полная тьма в  в 21
    uint32_t t1Sunset_step = 1;   // шаг заката
    uint32_t t1f2_dim = 50;       // яркость утром
    uint32_t t1f3_dim = 80;       // яркость обед
    uint32_t t1f4_dim = 70;       // яркость ужин

    // аквамен дозатор
    bool aquaDoz1isWorks = 0;
    uint32_t aquaDoz1_time = 0;

    //  из примеров Гайвера
    int number = 123456;
    String label = "label";
    bool led = 1;
    String paragr = "paragraph";
    String input = "input";
    char inputc[20] = "input";
    String pass = "pass";
    uint32_t color = 0xff0000;
    bool sw = true;
    uint32_t datew = 1728123055;
    uint32_t datetime = 1728123055;
    float slider = 33;
    uint8_t sel = 1;
};

extern Data data;  // объявляем что у нас будет переменная data класса Data

// constexpr size_t BRIGHT_SIZE = 100;// размер массива
// extern uint16_t brightn[BRIGHT_SIZE];
extern uint16_t brightn[101];

/*
byte red, green, blue;
unsigned long rgb = 0x6f56a3;

red = rgb >> 16
green = (rgb & 0x00ff00) >> 8;
blue = (rgb & 0x0000ff);

или



String hexstring = "B787B7";
    // long number = (long) strtol( &hexstring[1], NULL, 16);
    long number = (long) strtol( &hexstring[0], NULL, 16);
    int r = number >> 16;
    int g = number >> 8 & 0xFF;
    int b = number & 0xFF;
*/

// bool    t1Discr_enabled,
// uint32_t    t1Discr_startTime,
// uint32_t    t1Discr_endTime,

// bool    t2Discr_enabled,
// uint32_t    t2Discr_startTime,
// uint32_t    t2Discr_endTime,
// bool    t3Discr_enabled,
// uint32_t    t3Discr_startTime,
// uint32_t    t3Discr_endTime,

// bool    t4Discr_enabled,
// uint32_t    t4Discr_startTime,
// uint32_t    t4Discr_endTime,

// bool    t5Discr_enabled,
// uint32_t    t5Discr_startTime,
// uint32_t    t5Discr_endTime,
