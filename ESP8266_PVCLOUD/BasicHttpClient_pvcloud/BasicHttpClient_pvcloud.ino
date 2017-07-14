#include <Arduino.h>

/*
 * The folliwing secrets.h contains Strings with important secrets that must not go to public GIT
 * Content of the file goes as follows:

const char* WIFI_SSID = "";
const char* WIFI_PASS = "";
String PVCLOUD_KEY = "";
String PVCLOUD_APP_ID = "";
String PVCLOUD_API_URL = "";
String PVCLOUD_HTTPS_GET_URL = "";
String PVCLOUD_HTTPS_SERVER_FINGERPRINT = "";

 * 
 */
#include "secrets.h" 

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

void setup() {

    USE_SERIAL.begin(115200);
    // USE_SERIAL.setDebugOutput(true);

    pinMode(0, INPUT);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for (uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP(WIFI_SSID,WIFI_PASS);

}

void loop() {
    // wait for WiFi connection
    if ((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");

        int val = 0;
        val = digitalRead(0);

        if (val == HIGH) {
            http.begin(PVCLOUD_HTTPS_GET_URL, PVCLOUD_HTTPS_SERVER_FINGERPRINT); //HTTPS
            USE_SERIAL.print("[HTTP] GET...\n");
            // start connection and send HTTP header
            int httpCode = http.GET();

            // httpCode will be negative on error
            if (httpCode > 0) {
                // HTTP header has been send and Server response header has been handled
                USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

                // file found at server
                if (httpCode == HTTP_CODE_OK) {
                    String payload = http.getString();
                    USE_SERIAL.println(payload);
                }
            } else {
                USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            }

            http.end();

            delay(5000);
        }
        delay (500);
    }   
}

