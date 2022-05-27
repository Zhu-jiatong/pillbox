#if !defined(CONFIG_h)
#define CONFIG_h

#include <Arduino.h>

//#define DEBUG_MODE
#define NODEMCU

// pins and hardware
constexpr auto lcdAddr(0x27);
constexpr auto DHTTYPE(22); // DHT22
constexpr auto noOfAlarm(5);
constexpr auto ledPin(LED_BUILTIN);

#if defined(NANO_EVERY)
constexpr auto buttonPin(5);
constexpr auto DHTPIN(6);
#endif // NANO_EVERY
#if defined(NODEMCU) // do not use D1(SCL), D2(SDA), D14, D9, D10, D13, D11, D12
constexpr auto buttonPin(D7);
constexpr auto DHTPIN(D5);
constexpr char *ssid = "Pillbox"; // The name of the Wi-Fi network that will be created
#endif // NODEMCU

// time constants
constexpr short ledFreq(250);
constexpr unsigned long HMILLIS(3600000);
constexpr unsigned long MMILLIS(60000);
constexpr unsigned long SMILLIS(1000);
constexpr unsigned long DAYMILLIS(24 * HMILLIS);
constexpr short RTCINDX(0);

// LCD custom chars
uint8_t zero[]{B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000};
uint8_t one[]{B10000, B10000, B10000, B10000, B10000, B10000, B10000, B10000};
uint8_t two[]{B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000};
uint8_t three[]{B11100, B11100, B11100, B11100, B11100, B11100, B11100, B11100};
uint8_t four[]{B11110, B11110, B11110, B11110, B11110, B11110, B11110, B11110};
uint8_t five[]{B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};

// macro functions
#define arrElem(arr) (sizeof(arr) / sizeof(arr[0]))

#endif // CONFIG_h
