#include "ESP8266tools.hpp"
#include "Configuration.hpp"
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

ESP8266tools::ESP8266tools()
{
    _ssid = WIFI_SSID;
    _password = WIFI_PASSWORD;
    //_ssid= strdup(WIFI_SSID);
    //_password=strdup(WIFI_PASSWORD);
}

ESP8266tools::~ESP8266tools()
{
    _ssid = "";
    _password = "";
    //free(_ssid);
    //free(_password);
}

void ESP8266tools::esp8266info()
{
    Serial.printf("\n\n\nESP8266 INFORMATION\n===================\n");

    //ESP.getVcc() ⇒ may be used to measure supply voltage. ESP needs to reconfigure the ADC at startup in order for this feature to be available. ⇒ https://github.com/esp8266/Arduino/blob/master/doc/libraries.md#user-content-esp-specific-apis
    Serial.printf("ESP.getFreeHeap()              : %d\n", ESP.getFreeHeap()); //  returns the free heap size.
    Serial.printf("ESP.getChipId()                : 0x%X\n", ESP.getChipId()); //  returns the ESP8266 chip ID as a 32-bit integer.
    Serial.printf("ESP.getSdkVersion()            : %d\n", ESP.getSdkVersion());
    Serial.printf("ESP.getBootVersion()           : %d\n", ESP.getBootVersion());
    Serial.printf("ESP.getBootMode()              : %d\n", ESP.getBootMode());
    Serial.printf("ESP.getCpuFreqMHz()            : %d\n", ESP.getCpuFreqMHz());
    Serial.printf("ESP.getFlashChipId()           : 0x%X\n", ESP.getFlashChipId());
    Serial.printf("ESP.getFlashChipRealSize()     : %d\n", ESP.getFlashChipRealSize());
    Serial.printf("ESP.getFlashChipSize()         : %d\n", ESP.getFlashChipSize());  //returns the flash chip size, in bytes, as seen by the SDK (may be less than actual size).
    Serial.printf("ESP.getFlashChipSpeed()        : %d\n", ESP.getFlashChipSpeed()); // returns the flash chip frequency, in Hz.
    Serial.printf("ESP.getFlashChipMode()         : %d\n", ESP.getFlashChipMode());
    Serial.printf("ESP.getFlashChipSizeByChipId() : 0x%X\n", ESP.getFlashChipSizeByChipId());
    Serial.printf("ESP.getSketchSize()            : %d\n", ESP.getSketchSize());
    Serial.printf("ESP.getFreeSketchSpace()       : %d\n", ESP.getFreeSketchSpace());
    Serial.printf("ESP.getCycleCount()            : %d\n", ESP.getCycleCount()); // returns the cpu instruction cycle count since start as an unsigned 32-bit. This is useful for accurate timing of very short actions like bit banging.

    rst_info *xyz;
    Serial.printf("ESP.getResetInfoPtr()\n");
    xyz = ESP.getResetInfoPtr();
    Serial.println((*xyz).reason);
    Serial.println((*xyz).exccause);
    Serial.println((*xyz).epc1);
    Serial.println((*xyz).epc2);
    Serial.println((*xyz).epc3);
    Serial.println((*xyz).excvaddr);
    Serial.println((*xyz).depc);
}

/*
Si se usa memoria dinámica
char* ESP8266tools::ssid()
{
    return _ssid;
}

char* ESP8266tools::password()
{
    return _password;
}
*/

void ESP8266tools::setup_wifi()
{
    esp8266info();

    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(_ssid);
    
    // STA Mode: actua solo como cliente puesto que aun no se ha implementado la configuración/panel de control por HTTP
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
}

 void ESP8266tools::disconnectWifi()
 {
  WiFi.disconnect();
 }
