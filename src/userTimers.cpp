#include <Arduino.h>
#include "userTimers.h"
#include "settings.h"
#include "data.h"
#include "nastroyki.h"

void init_relays()
{
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
}

//
//
//
void userTimers()
{
    //=== таймер1
    if (db[kk::t1Discr_enabled].toBool())
    {
        if (db[kk::t1Discr_startTime].toInt() < db[kk::t1Discr_endTime].toInt()) // если нет перехода через полночь
        {
            if ((db[kk::t1Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t1Discr_endTime].toInt()))
            {
                if (!data.rel1_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_1, ON);
                    data.rel1_on = 1;
                }
            }
            else
            {
                if (data.rel1_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_1, OFF);
                    data.rel1_on = 0;
                }
            }
        }
        else if (db[kk::t1Discr_startTime].toInt() > db[kk::t1Discr_endTime].toInt()) // если есть переход через полночь
        {
            if ((db[kk::t1Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t1Discr_endTime].toInt()))
            {
                if (data.rel1_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_1, OFF);
                    data.rel1_on = 0;
                }
            }
            else
            {
                if (!data.rel1_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_1, ON);
                    data.rel1_on = 1;
                }
            }
        }
    }
    else
    {
        if (data.rel1_on) // если было включено, выключим
        {
            data.rel1_on = 0;
            digitalWrite(RELE_1, OFF);
        }
    }
    // таймер1 ===
    //=== таймер2
    if (db[kk::t2Discr_enabled].toBool())
    {
        if (db[kk::t2Discr_startTime].toInt() < db[kk::t2Discr_endTime].toInt()) // если нет перехода через полночь
        {
            if ((db[kk::t2Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t2Discr_endTime].toInt()))
            {
                if (!data.rel2_on) // avoid extra digWrite
                {

                    digitalWrite(RELE_2, ON);
                    data.rel2_on = 1;
                }
            }
            else
            {
                if (data.rel2_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_2, OFF);
                    data.rel2_on = 0;
                }
            }
        }
        else if (db[kk::t2Discr_startTime].toInt() > db[kk::t2Discr_endTime].toInt()) // если есть переход через полночь
        {
            if ((db[kk::t2Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t2Discr_endTime].toInt()))
            {
                if (data.rel2_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_2, OFF);
                    data.rel2_on = 0;
                }
            }
            else
            {
                if (!data.rel2_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_2, ON);
                    data.rel2_on = 1;
                }
            }
        }
    }
    else
    {
        if (data.rel2_on) // если было включено, выключим
        {
            data.rel2_on = 0;
            digitalWrite(RELE_2, OFF);
        }
    }
    // таймер2 ===
    //=== таймер3
    if (db[kk::t3Discr_enabled].toBool())
    {
        if (db[kk::t3Discr_startTime].toInt() < db[kk::t3Discr_endTime].toInt()) // если нет перехода через полночь
        {
            if ((db[kk::t3Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t3Discr_endTime].toInt()))
            {
                if (!data.rel3_on) // avoid extra digWrite
                {

                    digitalWrite(RELE_3, ON);
                    data.rel3_on = 1;
                }
            }
            else
            {
                if (data.rel3_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_3, OFF);
                    data.rel3_on = 0;
                }
            }
        }
        else if (db[kk::t3Discr_startTime].toInt() > db[kk::t3Discr_endTime].toInt()) // если есть переход через полночь
        {
            if ((db[kk::t3Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t3Discr_endTime].toInt()))
            {
                if (data.rel3_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_3, OFF);
                    data.rel3_on = 0;
                }
            }
            else
            {
                if (!data.rel3_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_3, ON);
                    data.rel3_on = 1;
                }
            }
        }
    }
    else
    {
        if (data.rel3_on) // если было включено, выключим
        {
            data.rel3_on = 0;
            digitalWrite(RELE_3, OFF);
        }
    }
    // таймер3 ===
    //=== таймер4
    if (db[kk::t4Discr_enabled].toBool())
    {
        if (db[kk::t4Discr_startTime].toInt() < db[kk::t4Discr_endTime].toInt()) // если нет перехода через полночь
        {
            if ((db[kk::t4Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t4Discr_endTime].toInt()))
            {
                if (!data.rel4_on) // avoid extra digWrite
                {

                    digitalWrite(RELE_4, ON);
                    data.rel4_on = 1;
                }
            }
            else
            {
                if (data.rel4_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_4, OFF);
                    data.rel4_on = 0;
                }
            }
        }
        else if (db[kk::t4Discr_startTime].toInt() > db[kk::t4Discr_endTime].toInt()) // если есть переход через полночь
        {
            if ((db[kk::t4Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t4Discr_endTime].toInt()))
            {
                if (data.rel4_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_4, OFF);
                    data.rel4_on = 0;
                }
            }
            else
            {
                if (!data.rel4_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_4, ON);
                    data.rel4_on = 1;
                }
            }
        }
    }
    else
    {
        if (data.rel4_on) // если было включено, выключим
        {
            data.rel4_on = 0;
            digitalWrite(RELE_4, OFF);
        }
    }
    // таймер4 ===
    //=== таймер5
    if (db[kk::t5Discr_enabled].toBool())
    {
        if (db[kk::t5Discr_startTime].toInt() < db[kk::t5Discr_endTime].toInt()) // если нет перехода через полночь
        {
            if ((db[kk::t5Discr_startTime].toInt() <= data.secondsNow) && (data.secondsNow <= db[kk::t5Discr_endTime].toInt()))
            {
                if (!data.rel5_on) // avoid extra digWrite
                {

                    digitalWrite(RELE_5, ON);
                    data.rel5_on = 1;
                }
            }
            else
            {
                if (data.rel5_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_5, OFF);
                    data.rel5_on = 0;
                }
            }
        }
        else if (db[kk::t5Discr_startTime].toInt() > db[kk::t5Discr_endTime].toInt()) // если есть переход через полночь
        {
            if ((db[kk::t5Discr_startTime].toInt() >= data.secondsNow) && (data.secondsNow >= db[kk::t5Discr_endTime].toInt()))
            {
                if (data.rel5_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_5, OFF);
                    data.rel5_on = 0;
                }
            }
            else
            {
                if (!data.rel5_on) // avoid extra digWrite
                {
                    digitalWrite(RELE_5, ON);
                    data.rel5_on = 1;
                }
            }
        }
    }
    else
    {
        if (data.rel5_on) // если было включено, выключим
        {
            data.rel5_on = 0;
            digitalWrite(RELE_5, OFF);
        }
    }
    // таймер5 ===

} // userTimers()