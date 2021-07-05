#include "main.h"

/* --------------------------------------------------------------------------------------------------------------- */

const char* ssid = "IrMa";  
const char* password = "4045041990";

RemoteDebug Debug;
BlynkTimer timer_Uptime;

/* --------------------------------------------------------------------------------------------------------------- */

void setup() {
  /* --- Wifi support --- */
  #ifdef WIFI_CONNECT_HOME
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        BLINK_BUILTIN(250);
        WiFi.begin(ssid, password);
        //Serial.println("Retrying connection...");
    }
    //IPAddress localIp = WiFi.localIP();
    //Serial.print("Start network with IP: ");
    //Serial.println(localIp);
  #endif

  /* --- OTA programming (working with WiFi support) --- */
  #ifdef OTA_PGM
    //OTA_PRINT.print("Starting OTA with HOSTNAME: ");
    //OTA_PRINT.print(OTA_HOSTNAME);
    //OTA_PRINT.print("...");
    ArduinoOTA.setHostname(OTA_HOSTNAME);
    ArduinoOTA.onStart([]() {
          //OTA_PRINT.println("Start updating ");
          BLINK_BUILTIN(0);
        }
    );
    ArduinoOTA.onEnd([]() {
          //OTA_PRINT.println("\nEnd");          
        }
    );
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        //OTA_PRINT.printf("Progress: %u%%\r", (progress / (total / 100)));
        BLINK_BUILTIN(0);
        }
    );
    ArduinoOTA.onError([](ota_error_t error) {
          //OTA_PRINT.printf("Error[%u]: ", error);
          if (error == OTA_AUTH_ERROR) {
            //OTA_PRINT.println("Auth Failed");
          } else if (error == OTA_BEGIN_ERROR) {
            //OTA_PRINT.println("Begin Failed");
          } else if (error == OTA_CONNECT_ERROR) {
            //OTA_PRINT.println("Connect Failed");
          } else if (error == OTA_RECEIVE_ERROR) {
            //OTA_PRINT.println("Receive Failed");
          } else if (error == OTA_END_ERROR) {
            //OTA_PRINT.println("End Failed");
          }
        }
    );
    ArduinoOTA.begin();
    //Debug.println("done.");
    BLINK_BUILTIN(1000);
  #endif

  	/* --- RemoteDebug --- */
  #ifdef TELNET_DEBUG
    Debug.begin(HOST_NAME); // Initialize the WiFi server
    Debug.setResetCmdEnabled(true); // Enable the reset command
    Debug.showProfiler(true); // Profiler (Good to measure times, to optimize codes)
    Debug.showColors(true); // Colors

    String hostNameWifi = HOST_NAME;
  hostNameWifi.concat(".local");

  #ifdef ESP8266 // Only for it
    WiFi.hostname(hostNameWifi);
  #endif

  #ifdef USE_MDNS  // Use the MDNS ?
    /*if (MDNS.begin(HOST_NAME)) {
        Debug.print("* MDNS responder started. Hostname -> ");
        Debug.println(HOST_NAME);
    }*/
    MDNS.begin(HOST_NAME);
    MDNS.addService("telnet", "tcp", 23);

  #endif
  #endif


  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password, BLYNK_SERVER_IP, 8080);
  timer_Uptime.setInterval(1000UL, localUptime);
  /* Check connection. Restart ESP if need */
  while(!Blynk.connected()) {
    for(int i = 0; i < 5; i ++) {
      BLINK_BUILTIN(500);
    }
    delay(1000);
    ESP.restart();
  }
}

/* --------------------------------------------------------------------------------------------------------------- */
uint32_t mLastTime;

void loop() {
  #ifdef OTA_PGM
    if(millis() - mLastTime >= DEBUG_MSG_INTERVAL) {
      Debug.print("Local uptime: ");
      Debug.println(millis() / 1000);
      Debug.print("Ralay state is ");
      String msg = (!digitalRead(RELAY_CONTROL_PIN)) ? "HI": "LOW";
      Debug.println(msg);
      mLastTime = millis();
    }
    ArduinoOTA.handle();
  #endif

  #ifdef TELNET_DEBUG
    Debug.handle();
  #endif
  
  Blynk.run();
  timer_Uptime.run();
}

/* --------------------------------------------------------------------------------------------------------------- */

void localUptime(void) {
  Blynk.virtualWrite(BPIN_UPTIME, millis() / 1000);
}

void ledBuiltinBlink(uint16 d) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(d);
  digitalWrite(LED_BUILTIN, HIGH);
}