#ifndef MyMQTTtools_HPP
#define MyMQTTtools_HPP

#include <Arduino.h>
#include <PubSubClient.h>
#include "Configuration.hpp"
#include "ESP8266tools.hpp"
#include <WiFiClient.h>

class MyMQTTtools
{
public:
    MyMQTTtools();
    ~MyMQTTtools();
    //char ssid();
    //char password();
    void setup();
    void reconnect();
    void callback(char *topic, byte *payload, int length);
    void keepalive();
    void publish(const char *topic, String message);
    void disconnectWifi();

private:
    // MQTT
    const char *_user;
    const char *_mqtt_password;
    const char *_mqtt_server;
    int _mqtt_port;

    //WiFi
    unsigned long connection_watchdog = 0;
    const char *_ssid;
    const char *_password;
};

#endif