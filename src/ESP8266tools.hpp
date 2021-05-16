#ifndef ESP8266tools_HPP
#define ESP8266tools_HPP

class ESP8266tools
{
public:
    ESP8266tools();
    ~ESP8266tools();
    //char ssid();
    //char password();
    void esp8266info();
    void setup_wifi();
    void disconnectWifi();
private:
    // No se si sería mejor declararlo como const, así no hay que hacer strdup ni free creo
    const char *_ssid;
    const char *_password;
};

#endif