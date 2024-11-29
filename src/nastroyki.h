// имя пароль вашей домашней сети
// можно ввести, подключившись к ESP AP c паролем 1234567890
#define WIFI ""
#define WIFIPASS ""
#define INDIKATOR 2 // на каком пине индикаторный светодиод

#define RELE_1 13
#define RELE_2 14
#define RELE_3 27
#define RELE_4 26
#define RELE_5 28
#define ON 0  // включение релюшек логическим нулем или единицей
#define OFF 1 //












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
