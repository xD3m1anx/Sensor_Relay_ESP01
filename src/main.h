//#define BLYNK_PRINT Debug
#define OTA_PRINT   Debug

#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <BlynkSimpleEsp8266.h>
#include <DHTesp.h>
#include "mydebug.h"
#include "esp8266gpio.h"

/* --------------------------------------------------------------------------------------------------------------- */

//#define WIFI_CONNECT_HOME             //WiFi connection ( without Blynk )
#define OTA_PGM                         //Over the Air programming
#define BUILTIN_LED_ON                  //Turn ON led indication in loading 
#define TELNET_DEBUG                    //Telnet debug

#define OTA_HOSTNAME                    "RelaySens"
#define HOST_NAME                       "relaysens"
#define BLYNK_SERVER_IP                 "192.168.0.50"
//#define BLYNK_AUTH_TOKEN                "M8BWp5UlO7yrru4TiaQ5wH2wPL7pxjOp"      //*.*.*.63
#define BLYNK_AUTH_TOKEN              "KJzgLjGgZD1JgR2U7rZJmW90pzQzZPWk"      //*.*.*.64

#define DEBUG_MSG_INTERVAL              (15000)                                 //ms 
#define RELAY_CONTROL_PIN               GPIO0
//#define BPIN_UPTIME                     V3                                      //*.*.*.63
#define BPIN_UPTIME                   V4                                      //*.*.*.64

/* --------------------------------------------------------------------------------------------------------------- */

#ifdef BUILTIN_LED_ON
  #define BLINK_BUILTIN(__delay__)    ledBuiltinBlink(__delay__)
#else
  #define BLINK_BUILTIN(any)     ;
  #pragma "Builtin led is off. To led on define 'BUILTIN_LED_ON'"  
#endif

/* --------------------------------------------------------------------------------------------------------------- */

void ledBuiltinBlink(uint16 d);
void localUptime(void);

/* --------------------------------------------------------------------------------------------------------------- */