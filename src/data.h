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

