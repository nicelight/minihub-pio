
#include "sensors.h"

#include "data.h"
#include "nastroyki.h"
#include "settings.h"

GyverDS18Single ds1(DS18B20_1);  // пин
GyverDS18Single ds2(DS18B20_2);  // пинDHT2PIN

DHT dht1(DHT1PIN, DHT1TYPE);
DHT dht2(DHT2PIN, DHT2TYPE);

void get1ds18() {
    if (ds1.ready()) {         // измерения готовы по таймеру
        if (ds1.readTemp()) {  // если чтение успешно
            Serial.print("temp1: ");
            // Serial.println(ds1.getTemp());
            data.floattDS1 = ds1.getTemp();
            data.temp_ds1x10 = (int)(data.floattDS1 * 10);
        } else {
            // Serial.println("error");
            data.floattDS1 = -80.0;
        }

        ds1.requestTemp();  // запрос следующего измерения
    }
}  // get1ds18()
//

void get2ds18() {
    if (ds2.ready()) {         // измерения готовы по таймеру
        if (ds2.readTemp()) {  // если чтение успешно
            Serial.print("temp2: ");
            // Serial.println(ds2.getTemp());
            data.floattDS2 = ds2.getTemp();
            data.temp_ds2x10 = (int)(data.floattDS2 * 10);
        } else {
            // Serial.println("error");
            data.floattDS2 = random(41, 44);
        }

        ds2.requestTemp();  // запрос следующего измерения
    }
}  // get2ds18()
//
//

void getdht1() {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht1.readHumidity();
    float t = dht1.readTemperature();
    // float hic = dht1.computeHeatIndex(t, h, false);  // Compute heat index in Celsius (isFahreheit = false)
    if (isnan(h) || isnan(t)) {
        data.hdht1 = -80;
        data.floattdht1 = -80.0;
    } else {
        data.hdht1 = (int)h;
        data.tdht1x10 = (int)(t * 10);
        data.floattdht1 = t;
    }
}  // getdht()

void getdht2() {
    float h = dht2.readHumidity();
    float t = dht2.readTemperature();
    if (isnan(h) || isnan(t)) {
        data.hdht2 = -80;
        data.floattdht2 = -80.0;
    } else {
        data.hdht2 = (int)h;
        data.floattdht2 = t;
    }
}  // getdht()
//
//
//