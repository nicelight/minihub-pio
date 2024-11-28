#include <Arduino.h>

// прошивка тут
//C:\Users\Acer\Documents\PlatformIO\Projects\minihub\.pio\build\esp32doit-devkit-v1

// имя пароль вашей домашней сети
// можно ввести, подключившись к ESP AP c паролем 1234567890
#define WIFI ""
#define WIFIPASS ""
#define INDIKATOR 2 // на каком пине индикаторный светодиод
#define RELE_1 13
#define RELE_2 14
#define ON 0 // включение релюшек логическим нулем или единицей
#define OFF 1 //

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

// обявление фкнций для их видимости из вкладок.

Timer each60Sec(60000); // таймер раз в минуту

LED indikator(INDIKATOR, 300, 3, 50, 20); // каждые 1000 милисек мигаем 3 раза каждых 50 мс, время горения 20 мсек

int valNum;
uint32_t startSeconds = 0;
uint32_t stopSeconds = 0;
byte initially = 10; // первых 10 секунд приписываем время в переменную

// // ключи для хранения в базе данных они же идентификаторы полей для веб морды
// enum kk : size_t
// {
//   kk,
//   txt,
//   pass,
//   uintw,
//   intw,
//   int64w,

//   color,
//   toggle,
//   slider,
//   selectw,

//   lbl1,
//   lbl2,

//   date,
//   t1f1_startTime,
//   datime,
//   secondsNow,

//   btn1,
//   btn2,
//   wifi_ssid,
//   wifi_pass,
//   apply
// };

// данные для работы бизнес логики
// struct Data
// {
//   uint32_t secondsNow = 12;
//   int number = 123456;

//   String label = "label";
//   bool led = 1;
//   String paragr = "paragraph";
//   String input = "input";
//   char inputc[20] = "input";
//   String pass = "pass";
//   uint32_t color = 0xff0000;
//   bool sw = true;
//   uint32_t datew = 1728123055;
//   uint32_t datetime = 1728123055;
//   float slider = 33;
//   uint8_t sel = 1;
// };
// Data data;

// // билдер! Тут строится наше окно настроек

// void build(sets::Builder &b)
// {
//   // // можно узнать, было ли действие по виджету
//   // if (b.build.isAction()) {
//   //   Serial.print("Set: 0x");
//   //   Serial.print(b.build.id, HEX);
//   //   Serial.print(" = ");
//   //   Serial.println(b.build.value);
//   // }
//   // обработка действий в интерфейсе:
//   switch (b.build.id)
//   {
//   case "intw"_h:
//     Serial.print("uintw_h: ");
//     Serial.println(b.build.value);
//     break;

//   case kk::int64w:
//     Serial.print("int64w: ");
//     Serial.println(b.build.value);
//     break;
//   }

//   {
//     sets::Group g(b, "Nicelight");
//     //        b.Time(kk::secondsNow, "Вермечко", sets::Colors::Red);
//     b.Time(kk::secondsNow, "Вермечко");
//     //        b.Time(kk::uptime, "Аптайм");
//     //    b.Time("", &data.secondsNow);// так было
//   }

//   // вне группы. Так тоже можно
//   b.Switch(kk::toggle, "тыр-тырка");

//   {
//     sets::Group g(b, "Основное освещение");
//     b.Time(kk::t1f1_startTime, "Рассвет");
//   }
//   {
//     sets::Group g(b, "Group 3");
//     b.Label(kk::lbl1, "lable1");
//     b.Label(kk::lbl2, "millis()", "", sets::Colors::Red);
//     b.Date(kk::date, "Date");
//     b.DateTime(kk::datime, "Datime");

//     b.Color(kk::color, "Color");
//     //    b.Switch(kk::toggle2, "Switch");
//     b.Select(kk::selectw, "Select", "var1;var2;hello");
//     b.Slider(kk::slider, "Slider", -10, 10, 0.5, "deg");
//   }

//   {
//     sets::Group g(b, "Group3");

//     {
//       sets::Menu g(b, "расширенные");
//       b.Input(kk::txt, "Text");
//       b.Pass(kk::pass, "Password");
//       b.Input(kk::uintw, "uint");

//       b.Input("intw"_h, "int");
//       // b.Input(kk::intw, "int");
//       b.Input(kk::int64w, "int 64");
//       // тут тоже могут быть группы
//       {
//         sets::Group g(b, "Group3");
//         b.Switch("sw1"_h, "switch 1");
//         b.Switch("sw2"_h, "switch 2");
//         b.Switch("sw3"_h, "switch 3");
//       }
//       {
//         sets::Group g(b, "настройки WiFi");
//         b.Input(kk::wifi_ssid, "SSID");
//         b.Pass(kk::wifi_pass, "Password");
//         if (b.Button(kk::apply, "Save & Restart"))
//         {
//           db.update(); // сохраняем БД не дожидаясь таймаута
//           WiFiConnector.connect(db[kk::wifi_ssid], db[kk::wifi_pass]);
//           notice_f = true;
//           //          ESP.restart();
//         } // button Save
//       } // настройки wifi

//       // и просто виджеты
//       b.Label("lbl3"_h, "Another label", "Val", sets::Colors::Green);
//       b.Label("lbl4"_h, "Привет", "Val", sets::Colors::Blue);

//       ////////////////////
//       // кнопки являются "групповым" виджетом, можно сделать несколько кнопок в одной строке
//       if (b.beginButtons())
//       {
//         // кнопка вернёт true при клике
//         if (b.Button(kk::btn1, "reload"))
//         {
//           Serial.println("reload");
//           b.reload();
//         }

//         if (b.Button(kk::btn2, "clear db", sets::Colors::Blue))
//         {
//           Serial.println("clear db");
//           db.clear();
//           db.update();
//         }
//         /////////////////////
//         b.endButtons(); // завершить кнопки
//       }
//     } // подменю

//   } // group3

// } // builder

// // это апдейтер. Функция вызывается, когда вебморда запрашивает обновления
// void update(sets::Updater &upd)
// {
//   // можно отправить значение по имени (хэшу) виджета
//   upd.update(kk::lbl1, random(100));
//   upd.update(kk::lbl2, millis());
//   upd.update(kk::secondsNow, data.secondsNow);
//   if (notice_f)
//   { // уведомление
//     notice_f = false;
//     upd.notice("Теперь подключитесь к вашей wifi сети и найдите устройство");
//     //    upd.alert("Ошибка");
//   }
//   // примечание: при ручных изменениях в базе данных отправлять новые значения не нужно!
//   // библиотека сделает это сама =)
// } // update

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
  //  db.init(kk::txt, "text");   // инициализация базы данных начальными значениями
  //  db.init(kk::pass, "some pass");
  //  db.init(kk::uintw, 64u);
  //  db.init(kk::intw, -10);
  //  db.init(kk::int64w, 1234567ll);
  //  db.init(kk::color, 0xff0000);
  //  db.init(kk::toggle, (bool)1);
  //  db.init(kk::selectw, (uint8_t)1);
  //  db.init(kk::date, 1719941932);
  //  db.init(kk::datime, 1719941932);
  db.init(kk::t1f1_startTime, 60);
  db.init(kk::slider, -3.5);

  db.init(kk::wifi_ssid, WIFI);
  db.init(kk::wifi_pass, WIFIPASS);
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
  pinMode(RELE_1, OUTPUT);
  digitalWrite(RELE_1, OFF);
  pinMode(RELE_2, OUTPUT);
  digitalWrite(RELE_2, OFF);

} // setup

void loop()
{
  WiFiConnector.tick(); // поддержка wifi связи
  sett.tick();          // поддержка веб интерфейса
  // NTP.tick();           // поддержка NTP
  indikator.tick(); // in loop

  if (each60Sec.ready())
  { // раз в минуту
    if (!NTP.status() && NTP.synced())
    {
      // берем текущую дату и время
      // db[kk::t1f1_startTime] =  //тут хранится время
      //    nowTime.set(ntp.hour(), ntp.minute(), ntp.second());
      //    nowDate.set(ntp.year(), ntp.month(), ntp.day());

      NTP.tick(); // поддержка NTP
      data.secondsNow = NTP.daySeconds();
    }
    else
      Serial.print("\n\n\tNTP discon\n\n");

  } // each60Sec

  if (NTP.newSecond())
  {
    if (initially--)
      data.secondsNow = NTP.daySeconds(); // вначале схватываем время с ntp
    data.secondsNow++;
  }
} // loop