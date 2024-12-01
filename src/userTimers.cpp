#include "userTimers.h"

#include <Arduino.h>

#include "data.h"
#include "nastroyki.h"
#include "settings.h"

void init_pins() {
    pinMode(RELE_1, OUTPUT);
    digitalWrite(RELE_1, OFF);
    pinMode(RELE_2, OUTPUT);
    digitalWrite(RELE_2, OFF);
    pinMode(RELE_3, OUTPUT);
    digitalWrite(RELE_3, OFF);
    pinMode(RELE_4, OUTPUT);
    digitalWrite(RELE_4, OFF);
    pinMode(RELE_5, OUTPUT);
    digitalWrite(RELE_5, OFF);
    pinMode(RELE_6, OUTPUT);
    digitalWrite(RELE_6, OFF);
    ledcSetup(0, 12000, 12);  // Канал 0
    ledcSetup(1, 12000, 12);  // Канал 1
    ledcSetup(2, 12000, 12);  // Канал 2

    // Привязка пинов к каналам
    ledcAttachPin(RED_RGB_LENTA, 0);    // Пин 0 к каналу 0
    ledcAttachPin(GREEN_RGB_LENTA, 1);  // Пин 2 к каналу 1
    ledcAttachPin(BLUE_RGB_LENTA, 2);   // Пин 4 к каналу 2

    /////     ВРЕМЕННО      ///////         УДАЛИТЬ !!!!!!!!!!!

    pinMode(GND_RGB_LENTA, OUTPUT);
    digitalWrite(GND_RGB_LENTA, 0);
}

///
void hueToRGB(uint8_t hue, uint16_t brightness) {
    uint16_t scaledHue = hue * 6;                          // Масштабируем оттенок в диапазон 0–1536
    uint8_t segment = scaledHue / 256;                     // Сегмент (0–5)
    uint16_t segmentOffset = scaledHue - (segment * 256);  // Смещение в сегменте (0–255)

    // uint32_t scaledSegmentOffset = (segmentOffset * 4095) / 255;     // Приведение segmentOffset к 12-битному диапазону
    uint32_t scaledSegmentOffset = segmentOffset * 16;

    uint32_t prev = (brightness * (4095 - scaledSegmentOffset)) / 4095;
    uint32_t next = (brightness * scaledSegmentOffset) / 4095;

    brightness = 4095 - brightness;  // Инверсия для лент с  общего анодом ( нам другие не подходят)
    prev = 4095 - prev;
    next = 4095 - next;
    switch (segment) {
        case 0:  // Красный
            RED_PWM = brightness;
            GREEN_PWM = next;
            BLUE_PWM = 0;
            break;
        case 1:  // Жёлтый
            RED_PWM = prev;
            GREEN_PWM = brightness;
            BLUE_PWM = 0;
            break;
        case 2:  // Зелёный
            RED_PWM = 0;
            GREEN_PWM = brightness;
            BLUE_PWM = next;
            break;
        case 3:  // Бирюзовый
            RED_PWM = 0;
            GREEN_PWM = prev;
            BLUE_PWM = brightness;
            break;
        case 4:  // Синий
            RED_PWM = next;
            GREEN_PWM = 0;
            BLUE_PWM = brightness;
            break;
        case 5:  // Фиолетовый
        default:
            RED_PWM = brightness;
            GREEN_PWM = 0;
            BLUE_PWM = prev;
            break;
    }
}
//
//
//
void userTimers() {
    //
    //
    //
    // ///////////////////              проверка ленты              ///////////////////
    // rgb_color++;
    // // Преобразование оттенка и яркости в RGB
    // hueToRGB(rgb_color, brightness);

    // // Управление каналами LEDC
    // ledcWrite(0, RED_PWM);    // Красный канал
    // ledcWrite(1, GREEN_PWM);  // Зелёный канал
    // ledcWrite(2, BLUE_PWM);   // Синий канал

    // delay(100);  // Задержка между изменениями
    //              //
    
    // if(rgb_color<100) rgb_color++;
    // else rgb_color = 0;
    // ledcWrite(0, brightn[rgb_color]);    // Красный канал
    // ledcWrite(1, brightn[100-rgb_color]);  // Зелёный канал
    // // ledcWrite(2, 4095);   // Синий канал

    // delay(100);  // Задержка между изменениями
                 //
                 //

    //=== таймер Реле1
    if (db[kk::t1Discr_enabled].toBool()) {
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
            data.rel1_on = 0;
            digitalWrite(RELE_1, OFF);
        }
    }
    // таймер1 ===
    //=== таймер Реле2
    if (db[kk::t2Discr_enabled].toBool()) {
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
            data.rel2_on = 0;
            digitalWrite(RELE_2, OFF);
        }
    }
    // таймер2 ===
    //=== таймер Реле3
    if (db[kk::t3Discr_enabled].toBool()) {
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
            data.rel3_on = 0;
            digitalWrite(RELE_3, OFF);
        }
    }
    // таймер3 ===
    //=== таймер Реле4
    if (db[kk::t4Discr_enabled].toBool()) {
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
    // таймер4 ===
    //=== таймер Реле 5
    if (db[kk::t5Discr_enabled].toBool()) {
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
            data.rel5_on = 0;
            digitalWrite(RELE_5, OFF);
        }
    }
    // таймер5 ===
    //=== таймер Реле 6
    if (db[kk::t6Discr_enabled].toBool()) {
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
            data.rel6_on = 0;
            digitalWrite(RELE_6, OFF);
        }
    }
    // таймер6 ===

    // Природное освещение
    if (db[kk::t1f_enabled].toBool()) {
    }

}  // userTimers()
