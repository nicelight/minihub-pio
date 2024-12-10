// #define DEBUG
// #define CHECKT1 // отладка глюков таймера природного освещения

// имя пароль вашей домашней сети
// можно ввести, подключившись к ESP AP c паролем 1234567890
#define WIFI ""
#define WIFIPASS ""
#define INDIKATOR 2 // на каком пине индикаторный светодиод

#define RELE_1 4
#define RELE_2 16
#define RELE_3 17
#define RELE_4 5
#define RELE_5 18
#define RELE_6 19

#define ON 0  // включение релюшек логическим нулем или единицей
#define OFF 1 //


//для rgb ленты
#define RED_RGB_LENTA 27
#define GREEN_RGB_LENTA 26
#define BLUE_RGB_LENTA 25
#define GND_RGB_LENTA 33 // удалить. дает минус на управление RGB транзисторами ленты 
#define RED_PWM_CHANNEL 0 
#define GREEN_PWM_CHANNEL 1 
#define BLUE_PWM_CHANNEL 2



#define PIN_DS18B20_1 4
#define PIN_DS18B20_2 16



#define PRIRODNIYSVET_BELIY











#ifndef RELE_1
#define RELE_1 LED_BUILTIN
#endif 
#ifndef RELE_2
#define RELE_2 LED_BUILTIN
#endif 
#ifndef RELE_3
#define RELE_3 LED_BUILTIN
#endif 
#ifndef RELE_4
#define RELE_4 LED_BUILTIN
#endif 
#ifndef RELE_5
#define RELE_5 LED_BUILTIN
#endif 
