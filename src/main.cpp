

// прошивка тут
// C:\Users\Acer\Documents\PlatformIO\Projects\minihub\.pio\build\esp32doit-devkit-v1

#include <Arduino.h>

#include "led.h"
#include "timer.h"
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <GyverDBFile.h>
#include <GyverDS18.h>
#include <GyverNTP.h>
#include <LittleFS.h>
#include <SettingsGyver.h>
#include <WiFiConnector.h>

// #include "driver/temp_sensor.h"
#include "data.h"  // тут лежит структура data по кошерному
#include "nastroyki.h"
#include "settings.h"
#include "userTimers.h"

// обявление фкнций для их видимости из вкладок.

Timer each5Sec(5000);    // таймер раз в минуту
Timer each5min(300000);  // таймер раз в 5 мин
Timer eachSec(1000);     // таймер раз в сек

LED indikator(INDIKATOR, 300, 3, 50, 20);  // каждые 1000 милисек мигаем 3 раза каждых 50 мс, время горения 20 мсек
bool gotWifi = 0;                          // если подключено было к сети, то пробуем реконектиться каждых 5 мин
int valNum;
uint32_t startSeconds = 0;
uint32_t stopSeconds = 0;
byte initially = 5;  // первых 10 секунд приписываем время в переменную

void setup() {
    each5min.rst();
    init_pins();

    Serial.begin(115200);
    Serial.println("\n\n\n\t\t\t ESP STARTED !\n\n");

    ds1.requestTemp();  // первый запрос на измерение DS18B20
    ds2.requestTemp();  // первый запрос на измерение DS18B20

    // ======== SETTINGS ========
    WiFi.mode(WIFI_AP_STA);  // режим AP_STA. Вызываем перед sett.begin(), чтобы settings знал о текущем режиме wifi
    sett.begin();
    sett.onBuild(build);
    sett.onUpdate(update);

    // ======== DATABASE ========
#ifdef ESP32
    LittleFS.begin(true);
#else
    LittleFS.begin();
#endif
    db.begin();
    db.init(kk::wifi_ssid, WIFI);
    db.init(kk::wifi_pass, WIFIPASS);
    db.init(kk::ntp_gmt, 5);
    db.init(kk::t1Discr_name, "Реле 1");
    db.init(kk::t1Discr_enabled, 0);
    db.init(kk::t1Discr_startTime, 21600ul);
    db.init(kk::t1Discr_endTime, 72000ul);
    db.init(kk::t2Discr_name, "Реле 2");
    db.init(kk::t2Discr_enabled, 0);
    db.init(kk::t2Discr_startTime, 21600ul);
    db.init(kk::t2Discr_endTime, 72000ul);
    db.init(kk::t3Discr_name, "Реле 3");
    db.init(kk::t3Discr_enabled, 0);
    db.init(kk::t3Discr_startTime, 21600ul);
    db.init(kk::t3Discr_endTime, 72000ul);
    db.init(kk::t4Discr_name, "Реле 4");
    db.init(kk::t4Discr_enabled, 0);
    db.init(kk::t4Discr_startTime, 21600ul);
    db.init(kk::t4Discr_endTime, 72000ul);
    db.init(kk::t5Discr_name, "Реле 5");
    db.init(kk::t5Discr_enabled, 0);
    db.init(kk::t5Discr_startTime, 21600ul);
    db.init(kk::t5Discr_endTime, 72000ul);
    db.init(kk::t6Discr_name, "Реле 6 недельное");
    db.init(kk::t6Discr_enabled, 0);
    db.init(kk::t6Discr_startTime, 21600ul);
    db.init(kk::t6Discr_endTime, 72000ul);
    db.init(kk::t6Discr_inMonday, 0);
    db.init(kk::t6Discr_inTuesday, 0);
    db.init(kk::t6Discr_inWensday, 0);
    db.init(kk::t6Discr_inThursday, 0);
    db.init(kk::t6Discr_inFriday, 0);
    db.init(kk::t6Discr_inSaturday, 0);
    db.init(kk::t6Discr_inSunday, 0);

    db.init(kk::t1f_enabled, (bool)0);
    db.init(kk::t1f1_startTime, 21600ul);
    db.init(kk::t1f2_startTime, 25200ul);
    db.init(kk::t1f2_dim, 70);
    db.init(kk::t1f3_startTime, 43200ul);
    db.init(kk::t1f3_dim, 95);
    db.init(kk::t1f4_startTime, 64800ul);
    db.init(kk::t1f4_dim, 80);
    db.init(kk::t1f5_startTime, 72000ul);
    db.init(kk::t1_stopTime, 75600ul);

    db.init(kk::aquaDoz1_enabled, 0);
    db.init(kk::aquaDoz1_1time, 25200ul);
    db.init(kk::aquaDoz1_2time, 43200ul);
    db.init(kk::aquaDoz1_need3rd, 0);
    db.init(kk::aquaDoz1_3time, 64800ul);
    db.init(kk::aquaDoz1_need4th, 0);
    db.init(kk::aquaDoz1_4time, 72000ul);
    db.init(kk::aquaDoz1_need5th, 0);
    db.init(kk::aquaDoz1_5time, 73000ul);
    db.init(kk::aquaDoz1_need6th, 0);
    db.init(kk::aquaDoz1_6time, 74000ul);
    db.init(kk::aquaDoz1_need7th, 0);
    db.init(kk::aquaDoz1_7time, 75000ul);
    db.init(kk::aquaDoz1_need8th, 0);
    db.init(kk::aquaDoz1_8time, 76000ul);
    db.init(kk::aquaDoze1_dozeTime, 59);

    db.init(kk::btnName, "имечко кнопоньки");
    db.init(kk::btnColor, 0xff00aa);
    db.dump(Serial);

    // первый запуск всех исполнительных механизмов
    data.timer_nature_applied = 1;  // запустим природное освещение
    data.t1f_enbl = db[kk::t1f_enabled];
    userNatureTimer();
    data.t1discr_enbl = db[kk::t1Discr_enabled];  // запустим суточные таймеры
    data.t2discr_enbl = db[kk::t2Discr_enabled];
    data.t3discr_enbl = db[kk::t3Discr_enabled];
    data.t4discr_enbl = db[kk::t4Discr_enabled];
    data.t5discr_enbl = db[kk::t5Discr_enabled];
    data.t6discr_enbl = db[kk::t6Discr_enabled];
    userSixTimers();

    // ======== WIFI ========
    // подключение и реакция на подключение или ошибку
    WiFiConnector.setPass("1234567890");  // пароль точки доступа
    WiFiConnector.setTimeout(10);         // сколько секунд пытаться приконнектиттся
    WiFiConnector.onConnect([]() {
        Serial.print("Con with IP: ");
        Serial.println(WiFi.localIP());
        indikator.setPeriod(3000, 1, 200, 150);  // раз в 000 сек, 0 раз взмигнем - по 00 милисек периоды, гореть будем 0 милисек
        gotWifi = 1;
        NTP.begin();
        NTP.setPeriod(600);  // обновлять раз в 600 сек
        NTP.tick();
        NTP.setGMT(db[kk::ntp_gmt]);
    });
    WiFiConnector.onError([]() {
        Serial.print("Error! start AP ");
        Serial.println(WiFi.softAPIP());
        indikator.setPeriod(600, 2, 100, 50);  // раз в  секунду два раза взмигнем - по 200 милисек, гореть будем 50 милисек
        //  по истечении 5 инут ребутаемся чтобы проверить вайфай
        if (each5min.ready() && gotWifi) {
            indikator.setPeriod(3000, 30, 100, 70);  // общее время, кол-во, период одного, один зажжен на.
            delay(3000);
            ESP.restart();
        }
    });

    WiFiConnector.connect(db[kk::wifi_ssid], db[kk::wifi_pass]);

}  // setup

void loop() {
    WiFiConnector.tick();  // поддержка wifi связи
    sett.tick();           // поддержка веб интерфейса
    NTP.tick();
    indikator.tick();  // in loop

    if (each5Sec.ready())  // раз в 5 сек
    {
        // поддержка NTP
        // делаем тут, а не в лупе,
        // чтобы при отпадении интернета все не зависало

        if (!NTP.status() && NTP.synced()) {
            data.secondsNow = NTP.daySeconds();
            curDataTime = NTP.getUnix();

        } else
            Serial.print("\n\n\tNTP not reached\n\n");

    }  // each60Sec

    if (eachSec.ready()) {  // раз в сек

        // if (initially)  // костыль для подхвата ntp, потому что если ntp отвалился, нельзя все время его чекать, мы его выше чекаем раз в минуту всего
        // {
        //     initially--;
        //     data.secondsNow = NTP.daySeconds();  // вначале схватываем с ntp
        // }
        data.secondsNow++;     // инкермент реалтайм
        data.secondsUptime++;  // инкермент аптайм
        if (data.secondsUptime == 86399){ // инкремент дней аптайма
            data.secondsUptime = 0;
            data.uptime_Days++;
        }
        userSixTimers();
        userNatureTimer();
        userFertiTimer();
    }

    // if(db.changed()){
    //   Serial.print("База изменена\t");
    //   Serial.println(millis());
    // }
    // static bool prevPinState = 0;
    // if(digitalRead(RELE_4) != prevPinState){
    //     prevPinState = digitalRead(RELE_4);
    //     Serial.print("RELE_4 = ");
    //     Serial.print(prevPinState);
    //     Serial.print("\n");
    // }

}  // loop