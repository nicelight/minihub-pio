
#include "sensors.h"

#include "data.h"
#include "nastroyki.h"
#include "settings.h"

GyverDS18Single ds1(PIN_DS18B20_1);  // пин
GyverDS18Single ds2(PIN_DS18B20_2);  // пин

DHT dht1(DHT1PIN, DHT1TYPE);

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

void getdht() {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht1.readHumidity();
    float t = dht1.readTemperature();
    // float hic = dht1.computeHeatIndex(t, h, false);  // Compute heat index in Celsius (isFahreheit = false)
    if (isnan(h) || isnan(t)) {
        data.hdht1 = -80;
        data.tdht1 = -80;
        data.floattdht1 = -80.0;
    } else {
        data.hdht1 = (int)h;
        data.tdht1 = (int)t;
        data.floattdht1 = t;
    }
}  // getdht()
//
//
//