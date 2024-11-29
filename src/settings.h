
#pragma once
#include <Arduino.h>
#include <GyverDBFile.h>
#include <SettingsGyver.h>

DB_KEYS( // ключи по которым идет привязка к базе и к виджетам в web
    kk,
    wifi_ssid,
    wifi_pass,
    apply,
    secondsNow,
    secondsUptime,
    t1Discr_name,
    t1Discr_enabled,
    t1Discr_startTime,
    t1Discr_endTime,
    t2Discr_name,
    t2Discr_enabled,
    t2Discr_startTime,
    t2Discr_endTime,
    t3Discr_name,
    t3Discr_enabled,
    t3Discr_startTime,
    t3Discr_endTime,
    t4Discr_name,
    t4Discr_enabled,
    t4Discr_startTime,
    t4Discr_endTime,
    t5Discr_name,
    t5Discr_enabled,
    t5Discr_startTime,
    t5Discr_endTime,
    
    t1f_enabled,
    t1f1_startTime,
    t1f2_startTime,
    t1f2_dim,
    t1f3_startTime,
    t1f3_dim,
    t1f4_startTime,
    t1f4_dim,
    t1f5_startTime,
    t1_stopTime,

    btnflex,
    btnName,
    btnColor,
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
    datime,

    btn1,
    btn2

);

static bool notice_f; // флаг на отправку уведомления о подключении к wifi
static bool rel1_on = 0; // флаг о том что реле включено
extern GyverDBFile db; // база данных для хранения настроек будет автоматически записываться в файл при изменениях
extern SettingsGyver sett;

// чтобы функции были видимыми, делаем их в *.cpp  не static
// и тут объявляем. Или же делаем их static ( не видимыми за пределами)
//  и тут не объявляем
void build(sets::Builder &b);
void update(sets::Updater &upd);