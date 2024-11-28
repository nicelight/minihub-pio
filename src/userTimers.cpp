#define RELE_1 13
#define RELE_2 14
#define ON 0  // включение релюшек логическим нулем или единицей
#define OFF 1 //

#include <Arduino.h>
#include "userTimers.h"
#include "data.h"
#include "settings.h"

void init_relays()
{
    pinMode(RELE_1, OUTPUT);
    digitalWrite(RELE_1, OFF);
    pinMode(RELE_2, OUTPUT);
    digitalWrite(RELE_2, OFF);
}

void userTimers()
{
    if (db[kk::t1Discr_enabled])
    {
        // Serial.println("Tmr1 enb");
        digitalWrite(RELE_1, ON);
    } else {
                digitalWrite(RELE_1, OFF);

    }
}