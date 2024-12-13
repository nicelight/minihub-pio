#include "userTimers.h"

#include <Arduino.h>

#include "data.h"
#include "nastroyki.h"
#include "settings.h"

GyverDS18Single ds1(PIN_DS18B20_1);  // пин
GyverDS18Single ds2(PIN_DS18B20_2);  // пин

static uint16_t RED_PWM, GREEN_PWM, BLUE_PWM;  // Значения компонентов Red, Green, Blue (0–4095)
static uint16_t brightness = 4095;             // Максимальная яркость (4095 для 12-битного PWM)
static byte t1fase = 0;                        // автомат состояний каждой фазы природного освещения
// static bool data.t1isWorks = 0;                     // прямо сейчас работает
static uint32_t t1fase_prevSeconds = 0;  // секунденый таймер для отрисовки природного освещения
static byte curr_sunrise_dim = 0;        // шаги яркости при рассвете
static byte curr_sunset_dim = 0;         // шаги яркости при закате

void init_pins() {
    pinMode(RELE_1, OUTPUT);
    digitalWrite(RELE_1, OFF);
    delay(100);  // чтоб не перегружать блоки питания и контакты
    pinMode(RELE_2, OUTPUT);
    digitalWrite(RELE_2, OFF);
    delay(100);
    pinMode(RELE_3, OUTPUT);
    digitalWrite(RELE_3, OFF);
    delay(100);
    pinMode(RELE_4, OUTPUT);
    digitalWrite(RELE_4, OFF);
    delay(100);
    pinMode(RELE_5, OUTPUT);
    digitalWrite(RELE_5, OFF);
    delay(100);
    pinMode(RELE_6, OUTPUT);
    digitalWrite(RELE_6, OFF);
    delay(100);

    // настройка ШИМ
    ledcSetup(RED_PWM_CHANNEL, 12000, 12);    // Канал 0
    ledcSetup(GREEN_PWM_CHANNEL, 12000, 12);  // Канал 1
    ledcSetup(BLUE_PWM_CHANNEL, 12000, 12);   // Канал 2

    // Привязка пинов к каналам
    ledcAttachPin(RED_RGB_LENTA, RED_PWM_CHANNEL);      // Пин 0 к каналу 0
    ledcAttachPin(GREEN_RGB_LENTA, GREEN_PWM_CHANNEL);  // Пин 2 к каналу 1
    ledcAttachPin(BLUE_RGB_LENTA, BLUE_PWM_CHANNEL);    // Пин 4 к каналу 2

    /////     ВРЕМЕННО      ///////         УДАЛИТЬ !!!!!!!!!!!
    pinMode(GND_RGB_LENTA, OUTPUT);
    digitalWrite(GND_RGB_LENTA, 0);
}  // init_pins()
//
//
//

// ЧИТАЕМ ИЗ БАЗЫ В RAM data.xxx
void read_t1_from_db() {
    // рассчет времени шага увеличения яркости при рассвете (в сек)
    if (!db[kk::t1f2_dim].toInt()) db[kk::t1f2_dim] = 1ul;
    data.t1Sunrise_step = (db[kk::t1f2_startTime].toInt() - db[t1f1_startTime].toInt()) / db[kk::t1f2_dim].toInt();
    if (!data.t1Sunrise_step) data.t1Sunrise_step = 1;  // чтобы не было нулем, если меньше единицы
    data.t1f1_time = db[kk::t1f1_startTime].toInt();    // рассвет с этого времени
    data.t1f2_time = db[kk::t1f2_startTime].toInt();    // утро с этого времени
    data.t1f3_time = db[kk::t1f3_startTime].toInt();    // обед с
    data.t1f4_time = db[kk::t1f4_startTime].toInt();    // ужин с
    data.t1f5_time = db[kk::t1f5_startTime].toInt();    // закат с
    data.t1f6_time = db[kk::t1_stopTime].toInt();       // полная тьма
    data.t1f2_dim = db[kk::t1f2_dim].toInt();           // яркость утром
    data.t1f3_dim = db[kk::t1f3_dim].toInt();           // яркость в обед
    data.t1f4_dim = db[kk::t1f4_dim].toInt();           // яркость вечером
    // шаг уменьшения яркости при закате (в секундах)
    if (!db[kk::t1f4_dim].toInt()) db[kk::t1f4_dim] = 1ul;
    data.t1Sunset_step = (db[kk::t1_stopTime].toInt() - db[t1f5_startTime].toInt()) / db[kk::t1f4_dim].toInt();
    if (!data.t1Sunset_step) data.t1Sunset_step = 1;  // чтобы не было нулем, если меньше единицы
}  // read_t1_from_db()
//
//
//

void getds18() {
    if (ds1.ready()) {         // измерения готовы по таймеру
        if (ds1.readTemp()) {  // если чтение успешно
            Serial.print("temp1: ");
            Serial.println(ds1.getTemp());
        } else {
            Serial.println("error");
        }

        ds1.requestTemp();  // запрос следующего измерения
    }
    if (ds2.ready()) {         // измерения готовы по таймеру
        if (ds2.readTemp()) {  // если чтение успешно
            Serial.print("temp2: ");
            Serial.println(ds2.getTemp());
        } else {
            Serial.println("error");
        }

        ds2.requestTemp();  // запрос следующего измерения
    }
}  // getds18()
//
//
//

void userSixTimers() {
    // таймер 1 ===
    // === таймер Реле 1
    // if (db[kk::t1Discr_enabled].toBool()) {
    if (data.t1discr_enbl) {
        if (db[kk::t1Discr_startTime].toInt() < db[kk::t1Discr_endTime].toInt())  // если нет перехода через полночь
        {
            if ((db[kk::t1Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t1Discr_endTime].toInt())) {
                if (!data.rel1_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_1, ON);
                    data.rel1_on = 1;
                }
            } else {
                if (data.rel1_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_1, OFF);
                    data.rel1_on = 0;
                }
            }
        } else if (db[kk::t1Discr_startTime].toInt() > db[kk::t1Discr_endTime].toInt())  // если есть переход через полночь
        {
            if ((db[kk::t1Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t1Discr_endTime].toInt())) {
                if (data.rel1_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_1, OFF);
                    data.rel1_on = 0;
                }
            } else {
                if (!data.rel1_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_1, ON);
                    data.rel1_on = 1;
                }
            }
        }
    } else {
        if (data.rel1_on)  // если было включено, выключим
        {
            digitalWrite(RELE_1, OFF);
            data.rel1_on = 0;
        }
    }
    // таймер 2 ===
    //=== таймер Реле 2
    // if (db[kk::t2Discr_enabled].toBool()) {
    if (data.t2discr_enbl) {
        if (db[kk::t2Discr_startTime].toInt() < db[kk::t2Discr_endTime].toInt())  // если нет перехода через полночь
        {
            if ((db[kk::t2Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t2Discr_endTime].toInt())) {
                if (!data.rel2_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_2, ON);
                    data.rel2_on = 1;
                }
            } else {
                if (data.rel2_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_2, OFF);
                    data.rel2_on = 0;
                }
            }
        } else if (db[kk::t2Discr_startTime].toInt() > db[kk::t2Discr_endTime].toInt())  // если есть переход через полночь
        {
            if ((db[kk::t2Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t2Discr_endTime].toInt())) {
                if (data.rel2_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_2, OFF);
                    data.rel2_on = 0;
                }
            } else {
                if (!data.rel2_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_2, ON);
                    data.rel2_on = 1;
                }
            }
        }
    } else {
        if (data.rel2_on)  // если было включено, выключим
        {
            digitalWrite(RELE_2, OFF);
            data.rel2_on = 0;
        }
    }
    // таймер 3 ===
    //=== таймер Реле 3
    // if (db[kk::t3Discr_enabled].toBool()) {
    if (data.t3discr_enbl) {
        if (db[kk::t3Discr_startTime].toInt() < db[kk::t3Discr_endTime].toInt())  // если нет перехода через полночь
        {
            if ((db[kk::t3Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t3Discr_endTime].toInt())) {
                if (!data.rel3_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_3, ON);
                    data.rel3_on = 1;
                }
            } else {
                if (data.rel3_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_3, OFF);
                    data.rel3_on = 0;
                }
            }
        } else if (db[kk::t3Discr_startTime].toInt() > db[kk::t3Discr_endTime].toInt())  // если есть переход через полночь
        {
            if ((db[kk::t3Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t3Discr_endTime].toInt())) {
                if (data.rel3_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_3, OFF);
                    data.rel3_on = 0;
                }
            } else {
                if (!data.rel3_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_3, ON);
                    data.rel3_on = 1;
                }
            }
        }
    } else {
        if (data.rel3_on)  // если было включено, выключим
        {
            digitalWrite(RELE_3, OFF);
            data.rel3_on = 0;
        }
    }
    // таймер 4 ===
    //=== таймер Реле4
    // if (db[kk::t4Discr_enabled].toBool()) {
    if (data.t4discr_enbl) {
        if (db[kk::t4Discr_startTime].toInt() < db[kk::t4Discr_endTime].toInt())  // если нет перехода через полночь
        {
            if ((db[kk::t4Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t4Discr_endTime].toInt())) {
                if (!data.rel4_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_4, ON);
                    data.rel4_on = 1;
                }
            } else {
                if (data.rel4_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_4, OFF);
                    data.rel4_on = 0;
                }
            }
        } else if (db[kk::t4Discr_startTime].toInt() > db[kk::t4Discr_endTime].toInt())  // если есть переход через полночь
        {
            if ((db[kk::t4Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t4Discr_endTime].toInt())) {
                if (data.rel4_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_4, OFF);
                    data.rel4_on = 0;
                }
            } else {
                if (!data.rel4_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_4, ON);
                    data.rel4_on = 1;
                }
            }
        }
    } else {
        if (data.rel4_on)  // если было включено, выключим
        {
            data.rel4_on = 0;
            digitalWrite(RELE_4, OFF);
        }
    }
    // таймер 5===
    //=== таймер Реле 5
    // if (db[kk::t5Discr_enabled].toBool()) {
    if (data.t5discr_enbl) {
        if (db[kk::t5Discr_startTime].toInt() < db[kk::t5Discr_endTime].toInt())  // если нет перехода через полночь
        {
            if ((db[kk::t5Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t5Discr_endTime].toInt())) {
                if (!data.rel5_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_5, ON);
                    data.rel5_on = 1;
                }
            } else {
                if (data.rel5_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_5, OFF);
                    data.rel5_on = 0;
                }
            }
        } else if (db[kk::t5Discr_startTime].toInt() > db[kk::t5Discr_endTime].toInt())  // если есть переход через полночь
        {
            if ((db[kk::t5Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t5Discr_endTime].toInt())) {
                if (data.rel5_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_5, OFF);
                    data.rel5_on = 0;
                }
            } else {
                if (!data.rel5_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_5, ON);
                    data.rel5_on = 1;
                }
            }
        }
    } else {
        if (data.rel5_on)  // если было включено, выключим
        {
            digitalWrite(RELE_5, OFF);
            data.rel5_on = 0;
        }
    }
    // таймер 6===
    //=== таймер Реле 6
    // if (db[kk::t6Discr_enabled].toBool()) {
    if (data.t6discr_enbl) {
        if (db[kk::t6Discr_startTime].toInt() < db[kk::t6Discr_endTime].toInt())  // если нет перехода через полночь
        {
            if ((db[kk::t6Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t6Discr_endTime].toInt())) {
                if (!data.rel6_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_6, ON);
                    data.rel6_on = 1;
                }
            } else {
                if (data.rel6_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_6, OFF);
                    data.rel6_on = 0;
                }
            }
        } else if (db[kk::t6Discr_startTime].toInt() > db[kk::t6Discr_endTime].toInt())  // если есть переход через полночь
        {
            if ((db[kk::t6Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t6Discr_endTime].toInt())) {
                if (data.rel6_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_6, OFF);
                    data.rel6_on = 0;
                }
            } else {
                if (!data.rel6_on)  // avoid extra digWrite
                {
                    digitalWrite(RELE_6, ON);
                    data.rel6_on = 1;
                }
            }
        }
    } else {
        if (data.rel6_on)  // если было включено, выключим
        {
            digitalWrite(RELE_6, OFF);
            data.rel6_on = 0;
        }
    }
    // таймер6 ===

}  // userSixTimers()
//
//
//

void userNatureTimer() {  //     // Природное освещение
    // нажали кнопку "Утвердить" в настройках природного освещения, или первый старт
    if (data.timer_nature_applied) {
        data.timer_nature_applied = 0;
        data.natureSec = data.secondsNow - 1;  // ускоряем срабатывание отрисовки света

        // проверка и корректировка настроек времен
        if (db[kk::t1f1_startTime].toInt() > 75600ul) db[kk::t1f1_startTime] = 28800ul;  // нельзя ставить поздний рассвет
        // каждая последующая фаза на 1мин  позже предыдущей
        if (db[kk::t1f2_startTime].toInt() <= db[kk::t1f1_startTime].toInt()) db[kk::t1f2_startTime] = db[kk::t1f1_startTime].toInt() + 60;
        if (db[kk::t1f3_startTime].toInt() <= db[kk::t1f2_startTime].toInt()) db[kk::t1f3_startTime] = db[kk::t1f2_startTime].toInt() + 60;
        if (db[kk::t1f4_startTime].toInt() <= db[kk::t1f3_startTime].toInt()) db[kk::t1f4_startTime] = db[kk::t1f3_startTime].toInt() + 60;
        if (db[kk::t1f5_startTime].toInt() <= db[kk::t1f4_startTime].toInt()) db[kk::t1f5_startTime] = db[kk::t1f4_startTime].toInt() + 60;
        if (db[kk::t1_stopTime].toInt() <= db[kk::t1f5_startTime].toInt()) db[kk::t1_stopTime] = db[kk::t1f5_startTime].toInt() + 60;
        // если яркость
        if (!db[kk::t1f2_dim].toInt()) db[kk::t1f2_dim] = 1;
        if (!db[kk::t1f3_dim].toInt()) db[kk::t1f3_dim] = 1;
        if (!db[kk::t1f4_dim].toInt()) db[kk::t1f4_dim] = 1;
        read_t1_from_db();  // прочитаем из базы в data.xxx все эти значения для природного освещения
        curr_sunrise_dim = 0;
        curr_sunset_dim = data.t1f4_dim;
    }  // data.timer_nature_applied

    // если включили работу таймера, каждую секунду отрисовываем
    if (data.t1f_enbl && (data.natureSec != data.secondsNow)) {
        data.natureSec = data.secondsNow;
        // if (db[kk::t1f_enabled]) { //заглючивает что то , не верно читается
#ifdef CHECKT1
        Serial.print("\n\t\tNATURE ENABLED\n");
#endif  // проверим в какой фазе мы сейчас
        // если не в ожидании и время за пределами работы света
        if (t1fase && ((data.secondsNow >= data.t1f6_time) || (data.secondsNow < data.t1f1_time))) {
            t1fase = 70;  //// автомат --- тушим и идем на ожидание
        }
        // рассвет
        if (((data.secondsNow >= data.t1f1_time) && (data.secondsNow < data.t1f2_time))) {
            t1fase = 10;
            // data.t1isWorks = 1;
        }
        // утро
        else if (((data.secondsNow >= data.t1f2_time) && (data.secondsNow < data.t1f3_time))) {
            t1fase = 20;
            // data.t1isWorks = 1;
        }
        // обед
        else if (((data.secondsNow >= data.t1f3_time) && (data.secondsNow < data.t1f4_time))) {
            t1fase = 30;
            // data.t1isWorks = 1;
        }
        // вечер
        else if (((data.secondsNow >= data.t1f4_time) && (data.secondsNow < data.t1f5_time))) {
            t1fase = 40;
            // data.t1isWorks = 1;
        }
        // закат
        else if (((data.secondsNow >= data.t1f5_time) && (data.secondsNow < data.t1f6_time))) {
            t1fase = 50;
            // Serial.println("Z A K A T ");
            // data.t1isWorks = 1;
        }
#ifdef DEBUGNATURELIGHT
        static byte prevFase = 100;
        if (prevFase != t1fase) {
            prevFase = t1fase;
            Serial.print("\n\tt1fase: ");
            Serial.print(t1fase);
            Serial.print("\t\t");
        }
#endif

        // автомат состояний природного освещения
        switch (t1fase) {
            case 0:
                curr_sunrise_dim = 0;
                curr_sunset_dim = data.t1f4_dim;
                t1fase_prevSeconds = data.secondsNow - 1000ul;  // ускорение первого шага
                break;
            case 10:  // рассвет
                // пока яркость не на максимуме
                if (curr_sunrise_dim < data.t1f2_dim) {
// ждем время инкремента яркости,  увеличиваем яркость на 1 позицию
#ifdef DEBUGNATURELIGHT
                    Serial.print("\nrise_dim: ");
                    Serial.print(curr_sunrise_dim);
                    Serial.print("\tsecNow: ");
                    Serial.print(data.secondsNow);
                    Serial.print("\tprevsec: ");
                    Serial.print(t1fase_prevSeconds);
                    Serial.print("\tstep: ");
                    Serial.print(data.t1Sunrise_step);
                    Serial.print("\tdata.secondsNow - t1fase_prevSeconds: ");
                    Serial.print(data.secondsNow - t1fase_prevSeconds);
#endif
                    if ((data.secondsNow - t1fase_prevSeconds) >= data.t1Sunrise_step) {
                        t1fase_prevSeconds = data.secondsNow;
                        curr_sunrise_dim++;
#ifdef DEBUG
                        Serial.print("\n1rise_dim: ");
                        Serial.print(curr_sunrise_dim);
                        Serial.print("\tnext in ");
                        Serial.print(data.t1Sunrise_step);
                        Serial.print(" sec..");
#endif
                        ledcWrite(RED_PWM_CHANNEL, brightn[curr_sunrise_dim]);
                        ledcWrite(GREEN_PWM_CHANNEL, brightn[curr_sunrise_dim >> 1]);
                        ledcWrite(BLUE_PWM_CHANNEL, brightn[curr_sunrise_dim >> 3]);
                        // ledcWrite(BLUE_PWM_CHANNEL, 0);

                    }  // if ms
                }  // step not max
                break;
            case 20:  // утро
#ifdef DEBUGNATURELIGHT
                Serial.print("\n1f2dim: ");
                Serial.print(data.t1f2_dim);
                Serial.print("\tbright: ");
                Serial.print(brightn[data.t1f2_dim]);
#endif
                ledcWrite(RED_PWM_CHANNEL, brightn[data.t1f2_dim]);
                ledcWrite(GREEN_PWM_CHANNEL, brightn[data.t1f2_dim]);
                ledcWrite(BLUE_PWM_CHANNEL, brightn[data.t1f2_dim]);
                break;
            case 30:  // обед
#ifdef DEBUG
                Serial.print("\n1f3dim: ");
                Serial.print(data.t1f3_dim);
                Serial.print("\tbright: ");
                Serial.print(brightn[data.t1f3_dim]);
#endif
                ledcWrite(RED_PWM_CHANNEL, brightn[data.t1f3_dim]);
                ledcWrite(GREEN_PWM_CHANNEL, brightn[data.t1f3_dim]);
                ledcWrite(BLUE_PWM_CHANNEL, brightn[data.t1f3_dim]);
                break;
            case 40:  //  вечер
#ifdef DEBUGNATURELIGHT
                Serial.print("\n1f4dim: ");
                Serial.print(data.t1f4_dim);
                Serial.print("\tbright: ");
                Serial.print(brightn[data.t1f4_dim]);
#endif
                ledcWrite(RED_PWM_CHANNEL, brightn[data.t1f4_dim]);
                ledcWrite(GREEN_PWM_CHANNEL, brightn[data.t1f4_dim]);
                ledcWrite(BLUE_PWM_CHANNEL, brightn[data.t1f4_dim]);
                curr_sunset_dim = data.t1f4_dim;
                break;
            case 50:  // закат
                // пока яркость не на минимуме
                if (curr_sunset_dim) {
#ifdef DEBUGNATURELIGHT
                    Serial.print("\nset_dim: ");
                    Serial.print(curr_sunset_dim);
                    Serial.print("\tsecNow: ");
                    Serial.print(data.secondsNow);
                    Serial.print("\tprevsec: ");
                    Serial.print(t1fase_prevSeconds);
                    Serial.print("\tstep: ");
                    Serial.print(data.t1Sunset_step);
                    Serial.print("\tdata.secondsNow - t1fase_prevSeconds: ");
                    Serial.print(data.secondsNow - t1fase_prevSeconds);
#endif
                    // ждем время декремента яркости,  увеличиваем яркость на 1 позицию
                    if ((data.secondsNow - t1fase_prevSeconds) >= data.t1Sunset_step) {
                        t1fase_prevSeconds = data.secondsNow;
                        curr_sunset_dim--;
#ifdef DEBUGNATURELIGHT
                        Serial.print("\nsunset_dim: ");
                        Serial.print(curr_sunset_dim);
                        Serial.print("\tnext step in: ");
                        Serial.print(data.t1Sunset_step);
                        Serial.print(" sec..");
#endif
                        ledcWrite(RED_PWM_CHANNEL, brightn[curr_sunset_dim]);
                        ledcWrite(GREEN_PWM_CHANNEL, brightn[curr_sunset_dim >> 1]);
                        ledcWrite(BLUE_PWM_CHANNEL, brightn[curr_sunset_dim >> 3]);
                        // ledcWrite(BLUE_PWM_CHANNEL, 0);

                    }  // if ms
                }  // step not max
                else {
                    t1fase = 70;
                }
                break;

            case 70:  // тушим свет, и на исходную
                // data.t1isWorks = 0;
                ledcWrite(RED_PWM_CHANNEL, 0);
                ledcWrite(GREEN_PWM_CHANNEL, 0);
                ledcWrite(BLUE_PWM_CHANNEL, 0);
                t1fase = 0;  // GO  на исходную
                break;
        }  // switch(t1fase)
    }  // timer enabled
    else {
        
    }
}  // userNatureTimer()
//
//
//

// таймер автодозатора от Аквамена
// https://www.youtube.com/watch?v=XGLbtAAbIi4&feature=youtu.be
