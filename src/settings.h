
#pragma once
#include <Arduino.h>
#include <GyverDBFile.h>
#include <GyverNTP.h>
#include <SettingsGyver.h>


// надо бы исправить DB_KEYS на нормальный enum чтобы конвертить можно было

// enum keys : size_t {
//     key1 = SH("key1"),
//     key2 = SH("key2"),
//     mykey = SH("mykey"),
// };

// Теперь enum хранит хэши и не боится удаления или добавления ключей не в конец. Для более короткой записи в библиотеке есть удобный макрос:

// DB_KEYS(keys,
//     key1,
//     key2,
//     mykey,
// );


DB_KEYS(  // ключи по которым идет привязка к базе и к виджетам в web
    kk,
    wifi_ssid,
    wifi_pass,
    apply,
    ntp_gmt,
    secondsNow,
    secondsUptime,
    dayofweek,
    uptimeDays,

    t1Discr_name,
    t1Discr_led,
    t1Discr_enabled,
    t1Discr_startTime,
    t1Discr_endTime,
    t2Discr_name,
    t2Discr_led,
    t2Discr_enabled,
    t2Discr_startTime,
    t2Discr_endTime,
    t3Discr_name,
    t3Discr_led,
    t3Discr_enabled,
    t3Discr_startTime,
    t3Discr_endTime,
    t4Discr_name,
    t4Discr_led,
    t4Discr_enabled,
    t4Discr_startTime,
    t4Discr_endTime,
    t5Discr_name,
    t5Discr_led,
    t5Discr_enabled,
    t5Discr_startTime,
    t5Discr_endTime,
    t6Discr_name,
    t6Discr_led,
    t6Discr_enabled,
    t6Discr_startTime,
    t6Discr_endTime,
    t6Discr_inMonday,
    t6Discr_inTuesday,
    t6Discr_inWensday,
    t6Discr_inThursday,
    t6Discr_inFriday,
    t6Discr_inSaturday,
    t6Discr_inSunday,

    t1f_enabled,
    t1f_led,
    t1f1_startTime,
    t1f2_startTime,
    t1f2_dim,
    t1f3_startTime,
    t1f3_dim,
    t1f4_startTime,
    t1f4_dim,
    t1f5_startTime,
    t1_stopTime,
    t1_btn_accept,

    aquaDoz1_enabled,
    aquaDoz1_nextDozeIn,
    aquaDoz1_led,
    aquaDoz1_1time,
    aquaDoz1_2time,
    aquaDoz1_need3rd,
    aquaDoz1_3time,
    aquaDoz1_need4th,
    aquaDoz1_4time,
    aquaDoz1_need5th,
    aquaDoz1_5time,
    aquaDoz1_need6th,
    aquaDoz1_6time,
    aquaDoz1_need7th,
    aquaDoz1_7time,
    aquaDoz1_need8th,
    aquaDoz1_8time,
    aquaDoze1_dozeTime,
    aquaDoz1_btn_accept,
    aquaDoz1_makeDoze,

    dht1name,
    floattemp1,

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
    btn2);

extern GyverDBFile db;  // база данных для хранения настроек будет автоматически записываться в файл при изменениях
extern SettingsGyver sett;

extern Datime curDataTime;

// чтобы функции были видимыми, делаем их в *.cpp  не static
// и тут объявляем. Или же делаем их static ( не видимыми за пределами)
//  и тут не объявляем
void build(sets::Builder &b);
void update(sets::Updater &upd);