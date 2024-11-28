#include <Arduino.h>

// прошивка тут
// C:\Users\Acer\Documents\PlatformIO\Projects\minihub\.pio\build\esp32doit-devkit-v1

// имя пароль вашей домашней сети
// можно ввести, подключившись к ESP AP c паролем 1234567890
#define WIFI ""
#define WIFIPASS ""
#define INDIKATOR 2 // на каком пине индикаторный светодиод

#include <Arduino.h>
#include "timer.h"
#include "led.h"
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <GyverDBFile.h>
#include <LittleFS.h>
#include <SettingsGyver.h>
#include <WiFiConnector.h>
#include <GyverNTP.h>
#include "data.h" // тут лежит структура data по кошерному
#include "settings.h"
#include "userTimers.h"

// обявление фкнций для их видимости из вкладок.

Timer each60Sec(60000); // таймер раз в минуту
Timer eachSec(1000);    // таймер раз в сек

LED indikator(INDIKATOR, 300, 3, 50, 20); // каждые 1000 милисек мигаем 3 раза каждых 50 мс, время горения 20 мсек

int valNum;
uint32_t startSeconds = 0;
uint32_t stopSeconds = 0;
byte initially = 5; // первых 10 секунд приписываем время в переменную

void setup()
{
  Serial.begin(115200);
  Serial.println();

  // ======== SETTINGS ========
  WiFi.mode(WIFI_AP_STA); // режим AP_STA. Вызываем перед sett.begin(), чтобы settings знал о текущем режиме wifi
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
  db.init(kk::t1Discr_enabled, 0);
  db.init(kk::t1Discr_startTime, 21600ul);
  db.init(kk::t1Discr_endTime, 72000ul);
  db.init(kk::t2Discr_enabled, 0);
  db.init(kk::t2Discr_startTime, 21600ul);
  db.init(kk::t2Discr_endTime, 72000ul);
  db.init(kk::t3Discr_enabled, 0);
  db.init(kk::t3Discr_startTime, 21600ul);
  db.init(kk::t3Discr_endTime, 72000ul);
  db.init(kk::t4Discr_enabled, 0);
  db.init(kk::t4Discr_startTime, 21600ul);
  db.init(kk::t4Discr_endTime, 72000ul);
  db.init(kk::t5Discr_enabled, 0);
  db.init(kk::t5Discr_startTime, 21600ul);
  db.init(kk::t5Discr_endTime, 72000ul);

  db.init(kk::t1f_enabled, 0);
  db.init(kk::t1f1_startTime, 19800ul);
  db.init(kk::t1f2_startTime, 19800ul);
  db.init(kk::t1f2_dim, 70);
  db.init(kk::t1f3_startTime, 19800ul);
  db.init(kk::t1f3_dim, 95);
  db.init(kk::t1f4_startTime, 19800ul);
  db.init(kk::t1f4_dim, 80);
  db.init(kk::t1f5_startTime, 19800ul);
  db.init(kk::t1_stopTime, 72000ul);
  //  db.init(kk::txt, "text");   // инициализация базы данных
  //  db.init(kk::slider, -3.5);
  //  db.init(kk::pass, "some pass");
  //  db.init(kk::uintw, 64u);
  //  db.init(kk::intw, -10);
  //  db.init(kk::int64w, 1234567ll);
  //  db.init(kk::color, 0xff0000);
  //  db.init(kk::toggle, (bool)1);
  //  db.init(kk::selectw, (uint8_t)1);
  //  db.init(kk::date, 1719941932);
  //  db.init(kk::datime, 1719941932);
  db.dump(Serial);

  // ======== WIFI ========
  // подключение и реакция на подключение или ошибку
  WiFiConnector.setPass("1234567890"); // пароль точки доступа
  WiFiConnector.setTimeout(10);        // сколько секунд пытаться приконнектиттся
  WiFiConnector.onConnect([]()
                          {
                            Serial.print("Connected! ");
                            Serial.println(WiFi.localIP());
                            indikator.setPeriod(3000, 1, 200, 150); // раз в 000 сек, 0 раз взмигнем - по 00 милисек периоды, гореть будем 0 милисек
                          });
  WiFiConnector.onError([]()
                        {
                          Serial.print("Error! start AP ");
                          Serial.println(WiFi.softAPIP());
                          indikator.setPeriod(600, 2, 100, 50); // раз в  секунду два раза взмигнем - по 200 милисек, гореть будем 50 милисек
                        });

  WiFiConnector.connect(db[kk::wifi_ssid], db[kk::wifi_pass]);

  NTP.begin(5);       // часовой пояс. Для Москвы: 3. Худжанд, Нск 5. Обновляться раз в 600 сек
  NTP.setPeriod(600); // обновлять раз в 600 сек
  NTP.tick();
  init_relays();

} // setup

void loop()
{
  WiFiConnector.tick(); // поддержка wifi связи
  sett.tick();          // поддержка веб интерфейса
  indikator.tick();     // in loop

  if (each60Sec.ready()) // раз в минуту
  {
    // поддержка NTP
    // делаем тут, а не в лупе,
    // чтобы при отпадении интернета все не зависало
    NTP.tick();

    if (!NTP.status() && NTP.synced())
    {
      // берем текущую дату и время
      // db[kk::t1f1_startTime] =  //тут хранится время
      //    nowTime.set(ntp.hour(), ntp.minute(), ntp.second());
      //    nowDate.set(ntp.year(), ntp.month(), ntp.day());

      data.secondsNow = NTP.daySeconds();
    }
    else
      Serial.print("\n\n\tNTP discon\n\n");

  } // each60Sec

  if (eachSec.ready())
  {                // раз в сек
    if (initially) // костыль для подхвата ntp, потому что если ntp отвалился, нельзя все время его чекать, мы его выше чекаем раз в минуту всего
    {
      initially--;
      data.secondsNow = NTP.daySeconds(); // вначале схватываем с ntp
    }
    data.secondsNow++;    // инкермент реалтайм
    data.secondsUptime++; // инкермент аптайм
  }

   userTimers();
} // loop