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

struct Data // обьявляем класс структуры
{
  uint32_t secondsNow = 12;
  uint32_t secondsUptime = 1;
  bool rel1_on = 0;
  bool rel2_on = 0;
  bool rel3_on = 0;
  bool rel4_on = 0;
  bool rel5_on = 0;
  
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

extern Data data; // объявляем что у нас будет переменная data класса Data

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
