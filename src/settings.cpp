#include "settings.h"

#include <GyverNTP.h>
#include <LittleFS.h>
#include <SettingsGyver.h>
#include <WiFiConnector.h>

#include "data.h"  // тут лежит структура data по кошерному
#include "nastroyki.h"
#include "userTimers.h"

// GyverDBFile db(&LittleFS, "/data.db");
// static SettingsESP sett(PROJECT_NAME " v" PROJECT_VER, &db);

GyverDBFile db(&LittleFS, "/nicelight.db");      // база данных для хранения настроек будет автоматически записываться в файл при изменениях
SettingsGyver sett("Горошек для любимой", &db);  // указывается заголовок меню, подключается база данных
bool timer_nature_applied = 1;                   // флаг на применение настроек природного освещения, кнопкой из веб морды отрабатывает
static bool notice_f;                            // флаг на отправку уведомления о подключении к wifi

// это апдейтер. Функция вызывается, когда вебморда запрашивает обновления
void update(sets::Updater &upd) {
    // отправляем свежие значения по имени (хэшу) виджета
    upd.update(kk::secondsNow, data.secondsNow);
    upd.update(kk::secondsUptime, data.secondsUptime);
    upd.update("lbl1"_h, random(100));
    upd.update("lbl2"_h, millis());
    upd.update("t1Discr_led"_h, data.rel1_on);
    upd.update("t2Discr_led"_h, data.rel2_on);
    upd.update("t3Discr_led"_h, data.rel3_on);
    upd.update("t4Discr_led"_h, data.rel4_on);
    upd.update("t5Discr_led"_h, data.rel5_on);
    upd.update("t6Discr_led"_h, data.rel6_on);
    upd.update("t1f_led"_h, data.t1isWorks);
    upd.update("aquaDoz1_led"_h, data.aquaDoz1isWorks);
    if (notice_f)  // уведомление при вводе wifi данных
    {
        notice_f = false;
        upd.notice("Если не ошибся с вводом, устройство подключится к  wifi сети, светодиод медленно замигает");
        //    upd.alert("Ошибка");
    }
    // примечание: при ручных изменениях в базе данных отправлять новые значения не нужно!
    // библиотека сделает это сама =)
}  // update

void build(sets::Builder &b) {
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

    // костыль на моментальное обновление индикаторных светодиодов
    switch (b.build.id) {
        case kk::t1Discr_startTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t1Discr_endTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t2Discr_startTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t2Discr_endTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t3Discr_startTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t3Discr_endTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t4Discr_startTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t4Discr_endTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t5Discr_startTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t5Discr_endTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t6Discr_startTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t6Discr_endTime:
            userSixTimers();
            b.reload();
            break;
        case kk::t1f1_startTime:
            userNatureTimer();
            b.reload();
            break;
        case kk::t1f2_startTime:
            userNatureTimer();
            b.reload();
            break;
        case kk::t1f3_startTime:
            userNatureTimer();
            b.reload();
            break;
        case kk::t1f4_startTime:
            userNatureTimer();
            b.reload();
            break;
        case kk::t1f5_startTime:
            userNatureTimer();
            b.reload();
            break;
        case kk::t1_stopTime:
            userNatureTimer();
            b.reload();
            break;
    }  //  switch (b.build.id)

    // WEB интерфейс ВЕБ морда формируется здесь
    {
        sets::Group g(b, "Nicelight");
        //        b.Time(kk::secondsNow, "Вермечко", sets::Colors::Red);
        // b.Time(kk::secondsUptime, "Аптайм", 0x3995db);
        b.Time(kk::secondsNow, "Вермечко");
        b.Time(kk::secondsUptime, "Аптайм");
    }
    /* суточные таймеры */
    {
        sets::Group g(b, "Суточные таймеры");
        // if (b.Switch(kk::t1Discr_enabled, " Реле света 1"))
        if (b.Switch(kk::t1Discr_enabled, db[kk::t1Discr_name], nullptr, sets::Colors::Yellow)) {  // Реле 1
            userSixTimers();
            b.reload();
        }
        if (db[kk::t1Discr_enabled].toBool()) {
            b.LED("t1Discr_led"_h, "Cтатус >>", data.rel1_on, sets::Colors::Black, sets::Colors::Yellow);
            // b.Time(kk::t1Discr_startTime, "Включается в ..", 0xf7e5b2);
            b.Time(kk::t1Discr_startTime, "Включается в ..");
            b.Time(kk::t1Discr_endTime, ".. и отключается в");
            b.Label(" ", " ");
        }
        // if (b.Switch(kk::t2Discr_enabled, " Реле света 2"))
        // if (b.Switch(kk::t2Discr_enabled, db[kk::t2Discr_name])) // Реле 2
        if (b.Switch(kk::t2Discr_enabled, db[kk::t2Discr_name], nullptr, sets::Colors::Green))  // Реле 2
        {
            b.reload();
            userSixTimers();
        }
        if (db[kk::t2Discr_enabled].toBool()) {
            b.LED("t2Discr_led"_h, "Cтатус >>", data.rel2_on, sets::Colors::Black, sets::Colors::Green);
            b.Time(kk::t2Discr_startTime, "Вкл в ..");
            b.Time(kk::t2Discr_endTime, ".. откл ");
            b.Label(" ", " ");
        }
        // if (b.Switch(kk::t3Discr_enabled, " Реле 3"))
        // if (b.Switch(kk::t3Discr_enabled, db[kk::t3Discr_name])) // Реле 3
        if (b.Switch(kk::t3Discr_enabled, db[kk::t3Discr_name], nullptr, sets::Colors::Mint))  // Реле 3
        {
            b.reload();
            userSixTimers();
        }
        if (db[kk::t3Discr_enabled].toBool()) {
            b.LED("t3Discr_led"_h, "Cтатус >>", data.rel3_on, sets::Colors::Black, sets::Colors::Mint);
            b.Time(kk::t3Discr_startTime, "Вкл в ..");
            b.Time(kk::t3Discr_endTime, ".. откл");
            b.Label(" ", " ");
            //    b.Time("", &data.secondsStart);// так было
        }
        // if (b.Switch(kk::t4Discr_enabled, " Реле 4"))
        // if (b.Switch(kk::t4Discr_enabled, db[kk::t4Discr_name])) // Реле 4
        if (b.Switch(kk::t4Discr_enabled, db[kk::t4Discr_name], nullptr, sets::Colors::Aqua))  // Реле 4
        {
            b.reload();
            userSixTimers();
        }
        if (db[kk::t4Discr_enabled].toBool()) {
            b.LED("t4Discr_led"_h, "Cтатус >>", data.rel4_on, sets::Colors::Black, sets::Colors::Aqua);
            b.Time(kk::t4Discr_startTime, "Вкл в ..");
            b.Time(kk::t4Discr_endTime, ".. откл");
            b.Label(" ", " ");
            //    b.Time("", &data.secondsStart);// так было
        }
        // if (b.Switch(kk::t5Discr_enabled, " Реле 5"))
        // if (b.Switch(kk::t5Discr_enabled, db[kk::t5Discr_name])) // Реле 5
        if (b.Switch(kk::t5Discr_enabled, db[kk::t5Discr_name], nullptr, sets::Colors::Blue))  // Реле 5
        {
            b.reload();
            userSixTimers();
        }
        if (db[kk::t5Discr_enabled].toBool()) {
            b.LED("t5Discr_led"_h, "Cтатус >>", data.rel5_on, sets::Colors::Black, sets::Colors::Blue);
            b.Time(kk::t5Discr_startTime, "Вкл в ..");
            b.Time(kk::t5Discr_endTime, ".. откл");
            b.Label(" ", " ");
        }
        // if (b.Switch(kk::t6Discr_enabled, " Реле 6"))
        // if (b.Switch(kk::t6Discr_enabled, db[kk::t6Discr_name])) // Реле 6
        if (b.Switch(kk::t6Discr_enabled, db[kk::t6Discr_name], nullptr, sets::Colors::Violet))  // Реле 6
        {
            b.reload();
            userSixTimers();
        }
        if (db[kk::t6Discr_enabled].toBool()) {
            b.LED("t6Discr_led"_h, "Cтатус >>", data.rel6_on, sets::Colors::Black, sets::Colors::Violet);
            b.Time(kk::t6Discr_startTime, "Вкл в ..");
            b.Time(kk::t6Discr_endTime, ".. откл");
            b.Label(" ", " ");
            b.Label(" ", " ");
        }
        // // костыль чтобы индикаторные светодиодики загорались сразу
        // if (data.updateInterface) {
        //     data.updateInterface = 0;
        //     b.reload();
        // }

    } /* суточные таймеры */

    /* аквариумистика */
    {
        sets::Group g(b, "Природное освещение");
        if (b.Switch(kk::t1f_enabled, "Сделать красиво", nullptr, sets::Colors::Orange)) {
            Serial.print("\n\t\t\t Switch Природное enabled\n\n");
            userNatureTimer();
            b.reload();
        }
        if (db[kk::t1f_enabled]) {
        // if (db[kk::t1f_enabled].toBool()) {
            b.LED("t1f_led"_h, "Cтатус >>", data.t1isWorks, sets::Colors::Black, sets::Colors::Orange);
            b.Time(kk::t1f1_startTime, "Рассвет начинается с");
            b.Time(kk::t1f2_startTime, "Утро с");
            b.Slider(kk::t1f2_dim, "яркость утром");
            b.Time(kk::t1f3_startTime, "День с");
            b.Slider(kk::t1f3_dim, "яркость днем");
            b.Time(kk::t1f4_startTime, "Вечер  с");
            b.Slider(kk::t1f4_dim, "яркость вечером");
            b.Time(kk::t1f5_startTime, "Закат начинается");
            b.Time(kk::t1_stopTime, "полная тьма к");
            if (b.Button(kk::t1_btn_accept, "Обновить", sets::Colors::Orange)) {
                //  Serial.println("\n\n\tApplied\n\n");
                timer_nature_applied = 1;
                userNatureTimer();
                b.reload();
            }
            b.Label(" ", " ");

        }  // if enabled
    }  // природное освещение
    /* аквариумистика */

    /* аквамен дозатор шприцы */
    // TODO
    // допилить userTImers
    // добавить в базу состояние вклю выкл и временные шняги
    {
        sets::Group g(b, "Мультитаймер");
        if (b.Switch(kk::aquaDoz1_enabled, "Дозатор удобрений", nullptr, sets::Colors::Aqua)) {
            // userNatureTimer();
            b.reload();     
        }
        if (db[kk::aquaDoz1_enabled].toBool()) {
            b.LED("aquaDoz1_led"_h, "Cтатус >>", data.aquaDoz1isWorks, sets::Colors::Black, sets::Colors::Aqua);
            b.Time(kk::aquaDoz1_1time, "Первый раз в");
            b.Time(kk::aquaDoz1_2time, "Второй раз в");
            if (b.Switch(kk::aquaDoz1_need3rd, "Еще подача", nullptr, sets::Colors::Aqua)) {
                // userNatureTimer();
                b.reload();
            }
            if (db[kk::aquaDoz1_need3rd].toBool())
                b.Time(kk::aquaDoz1_3time, "в");

            if (b.Switch(kk::aquaDoz1_need4th, "Еще подача", nullptr, sets::Colors::Aqua)) {
                // userNatureTimer();
                b.reload();
            }
            if (db[kk::aquaDoz1_need4th].toBool())
                b.Time(kk::aquaDoz1_4time, "в");

            if (b.Switch(kk::aquaDoz1_need5th, "Еще подача", nullptr, sets::Colors::Aqua)) {
                // userNatureTimer();
                b.reload();
            }
            if (db[kk::aquaDoz1_need5th].toBool())
                b.Time(kk::aquaDoz1_5time, "в");
            if (b.Switch(kk::aquaDoz1_need6th, "Еще подача", nullptr, sets::Colors::Aqua)) {
                // userNatureTimer();
                b.reload();
            }
            if (db[kk::aquaDoz1_need6th].toBool())
                b.Time(kk::aquaDoz1_6time, "в");
            if (b.Switch(kk::aquaDoz1_need7th, "Еще подача", nullptr, sets::Colors::Aqua)) {
                // userNatureTimer();
                b.reload();
            }
            if (db[kk::aquaDoz1_need7th].toBool())
                b.Time(kk::aquaDoz1_7time, "в");
            if (b.Switch(kk::aquaDoz1_need8th, "Еще подача", nullptr, sets::Colors::Aqua)) {
                // userNatureTimer();
                b.reload();
            }
            if (db[kk::aquaDoz1_need8th].toBool())
                b.Time(kk::aquaDoz1_8time, "в");


            b.Time(kk::aquaDoze1_dozeTime, "Подача дозы в течении");
            if (b.Button(kk::aquaDoz1_btn_accept, "Обновить", sets::Colors::Aqua)) {
                //  Serial.println("\n\n\tApplied\n\n");
                // timer_nature_applied = 1;
                // userNatureTimer();
                b.reload();
            }
         
            b.Label(" ", " ");
            if (b.Button(kk::aquaDoz1_makeDoze, "Дать дозу", sets::Colors::Gray)) {
                //  Serial.println("\n\n\tApplied\n\n");
                // timer_nature_applied = 1;
                // userNatureTimer();
                b.reload();
            }
         
            b.Label(" ", " ");

        }  // if enabled
    } /* аквамен дозатор шприцы */

    /* Настройки , внизу страницы*/
    {
        sets::Group g(b, " ");
        {
            sets::Menu g(b, "Опции");

            {
                sets::Menu g(b, "Интерфейс");
                /*
                      //пример изменения имени виджета
                      b.Input(kk::btnName, "новое имя кнопки:");
                      if(b.Button(kk::btnflex, db[kk::btnName], db[kk::btnColor])) b.reload();
                */

                b.Input(kk::t1Discr_name, "Имя Реле1:");
                b.Input(kk::t2Discr_name, "Имя Реле2:");
                b.Input(kk::t3Discr_name, "Имя Реле3:");
                b.Input(kk::t4Discr_name, "Имя Реле4:");
                b.Input(kk::t5Discr_name, "Имя Реле5:");
                b.Input(kk::t6Discr_name, "Имя Реле6:");
            }
            {
                sets::Menu g(b, "Расширенные");
                /// провалились в расширенные пристройки
                {
                    sets::Group g(b, "настройки WiFi");
                    b.Input(kk::wifi_ssid, "SSID");
                    b.Pass(kk::wifi_pass, "Password");
                    if (b.Button(kk::apply, "Save & Restart")) {
                        db.update();  // сохраняем БД не дожидаясь таймаута
                        WiFiConnector.connect(db[kk::wifi_ssid], db[kk::wifi_pass]);
                        notice_f = true;
                        //          ESP.restart();
                    }  // button Save
                }  // настройки wifi

                // и просто виджеты
                b.Label("lbl3"_h, "Another label", "Val", sets::Colors::Green);
                b.Label("lbl4"_h, "Привет", "Val", sets::Colors::Blue);

                // кнопки являются "групповым" виджетом, можно сделать несколько кнопок в одной строке
                if (b.beginButtons()) {
                    // кнопка вернёт true при клике
                    if (b.Button(kk::btn1, "reload")) {
                        Serial.println("reload");
                        b.reload();
                    }

                    if (b.Button(kk::btn2, "стереть базу(нет)", sets::Colors::Blue)) {
                        Serial.println("could clear db");
                        // db.clear();
                        // db.update();
                    }
                    b.endButtons();  // завершить кнопки
                }
            }  // Расширенные

            {
                sets::Menu g(b, "примеры");
                b.Input(kk::txt, "Text");
                b.Pass(kk::pass, "Password");
                b.Input(kk::uintw, "uint");

                {
                    sets::Group g(b, "групка");
                    // пример изменения имени виджета
                    b.Input(kk::btnName, "новое имя кнопки:");
                    if (b.Button(kk::btnflex, db[kk::btnName], db[kk::btnColor]))
                        b.reload();
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

                // вне группы. Так тоже можно
                b.Switch(kk::toggle, "тыр-тырка");
            }  // Menu-примеры
        }
    }  // Подстройки

}  // builder
