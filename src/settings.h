
#pragma once
#include <Arduino.h>
#include <GyverDBFile.h>
#include <SettingsGyver.h>

DB_KEYS(
    kk,
    txt,
    pass,
    uintw,
    intw,
    int64w,

    color,
    toggle,
    slider,
    selectw,

    lbl1,
    lbl2,

    date,
    t1f1_startTime,
    datime,
    secondsNow,

    btn1,
    btn2,
    wifi_ssid,
    wifi_pass,
    apply

);

static bool notice_f; // флаг на отправку уведомления о подключении к wifi

extern GyverDBFile db; // база данных для хранения настроек будет автоматически записываться в файл при изменениях
extern SettingsGyver sett;

// чтобы функции были видимыми, делаем их в *.cpp  не static 
// и тут объявляем. Или же делаем их static ( не видимыми за пределами)
//  и тут не объявляем  
void build(sets::Builder &b); 
void update(sets::Updater &upd);