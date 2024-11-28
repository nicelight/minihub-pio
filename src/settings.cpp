#include "data.h" // тут лежит структура data по кошерному
#include "settings.h"

#include <GyverNTP.h>
#include <LittleFS.h>
#include <SettingsGyver.h>
#include <WiFiConnector.h>

// GyverDBFile db(&LittleFS, "/data.db");
// static SettingsESP sett(PROJECT_NAME " v" PROJECT_VER, &db);

GyverDBFile db(&LittleFS, "/nicelight.db");     // база данных для хранения настроек будет автоматически записываться в файл при изменениях
SettingsGyver sett("Горошек для любимой", &db); // указывается заголовок меню, подключается база данных

// это апдейтер. Функция вызывается, когда вебморда запрашивает обновления
void update(sets::Updater &upd)
{
  // отправляем свежие значения по имени (хэшу) виджета
  upd.update(kk::secondsNow, data.secondsNow);
  upd.update(kk::secondsUptime, data.secondsUptime);
  upd.update("lbl1"_h, random(100));
  upd.update("lbl2"_h, millis());
  if (notice_f) // уведомление при вводе wifi данных
  {
    notice_f = false;
    upd.notice("Если не ошибся с вводом, устройство подключится к  wifi сети, светодиод медленно замигает");
    //    upd.alert("Ошибка");
  }
  // примечание: при ручных изменениях в базе данных отправлять новые значения не нужно!
  // библиотека сделает это сама =)
} // update

void build(sets::Builder &b)
{
  /*
    // можно узнать, было ли действие по виджету
    if (b.build.isAction()) {
      Serial.print("Set: 0x");
      Serial.print(b.build.id, HEX);
      Serial.print(" = ");
      Serial.println(b.build.value);
    }
  */

  /*
    // обработка действий от виджетов:
    switch (b.build.id)
    {
    case "uintw"_h: // если ввели импут
      Serial.print("input: ");
      Serial.println(b.build.value);
      break;

    case "intw"_h:
      Serial.print("intw ");
      Serial.println(b.build.value);
      break;
    }
  */

  // 0x3995db //цвет интерфейса зоны отдыха голубенький
  // 0xdb7d39 //цвет интерфейса игры оранжевый
  // 0xaa39db // дискотека фиолетовый

  {
    sets::Group g(b, "Nicelight");
    //        b.Time(kk::secondsNow, "Вермечко", sets::Colors::Red);
    // b.Time(kk::secondsUptime, "Аптайм", 0x3995db);
    b.Time(kk::secondsNow, "Вермечко");
    b.Time(kk::secondsUptime, "Аптайм");
    }

  {
    sets::Group g(b, "освещение Вкл/Выкл ");
    b.Time(kk::t1Discr_startTime, "Реле света включается в");
    b.Time(kk::t1Discr_endTime, ".. и включается в");
    //    b.Time("", &data.secondsStart);// так было
  }
  {
    sets::Group g(b, "Природное освещение");
    b.Time(kk::t1f1_startTime, "Рассвет начинается с");
    b.Time(kk::t1f2_startTime, "Утро с");

    // b.Slider(Text label = "", float min = 0, float max = 100, float step = 1, Text unit = Text(), AnyPtr value = nullptr);

    b.Slider(kk::t1f2_dim, "яркость утром");
    b.Time(kk::t1f3_startTime, "День с");
    b.Slider(kk::t1f3_dim, "яркость днем");
    b.Time(kk::t1f4_startTime, "Вечер  с");
    b.Slider(kk::t1f4_dim, "яркость вечером");
    b.Time(kk::t1f5_startTime, "Закат начинается");
    b.Time(kk::t1_stopTime, "полная тьма к");
  }

  // вне группы. Так тоже можно
  b.Switch(kk::toggle, "тыр-тырка");
  {
    sets::Group g(b, "Group 3");
    b.Label(kk::lbl1, "lable1");
    b.Label(kk::lbl2, "millis()", "", sets::Colors::Red);
    b.Date(kk::date, "Date");
    b.DateTime(kk::datime, "Datime");

    b.Color(kk::color, "Color");
    //    b.Switch(kk::toggle2, "Switch");
    b.Select(kk::selectw, "Select", "var1;var2;hello");
    b.Slider(kk::slider, "Slider", -10, 10, 0.5, "deg");
  }

  {
    sets::Group g(b, "Group3");
    {
      sets::Menu g(b, "расширенные");
      b.Input(kk::txt, "Text");
      b.Pass(kk::pass, "Password");
      b.Input(kk::uintw, "uint");

      {
        sets::Group g(b, "грвГр");
        b.Input("intw"_h, "int");
        b.Switch("sw1"_h, "switch 1");
      }
      //      b.Input(kk::intw, "int");
      b.Input(kk::int64w, "int 64");
      // тут тоже могут быть группы
      {
        sets::Group g(b, "тумблерки");
        b.Switch("sw2"_h, "switch 2");
        b.Switch("sw3"_h, "switch 3");
      }
      {
        sets::Group g(b, "настройки WiFi");
        b.Input(kk::wifi_ssid, "SSID");
        b.Pass(kk::wifi_pass, "Password");
        if (b.Button(kk::apply, "Save & Restart"))
        {
          db.update(); // сохраняем БД не дожидаясь таймаута
          WiFiConnector.connect(db[kk::wifi_ssid], db[kk::wifi_pass]);
          notice_f = true;
          //          ESP.restart();
        } // button Save
      } // настройки wifi

      // и просто виджеты
      b.Label("lbl3"_h, "Another label", "Val", sets::Colors::Green);
      b.Label("lbl4"_h, "Привет", "Val", sets::Colors::Blue);

      ////////////////////
      // кнопки являются "групповым" виджетом, можно сделать несколько кнопок в одной строке
      if (b.beginButtons())
      {
        // кнопка вернёт true при клике
        if (b.Button(kk::btn1, "reload"))
        {
          Serial.println("reload");
          b.reload();
        }

        if (b.Button(kk::btn2, "clear db", sets::Colors::Blue))
        {
          Serial.println("clear db");
          db.clear();
          db.update();
        }
        /////////////////////
        b.endButtons(); // завершить кнопки
      }
    } // подменю

  } // group3

} // builder
