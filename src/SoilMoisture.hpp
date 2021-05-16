#ifndef SOILMOISTURE_HPP
#define SOILMOISTURE_HPP

#include <WiFiClient.h>

class SoilMoisture
{
public:
    SoilMoisture();
    ~SoilMoisture();
    //char ssid();
    //char password();
    int read();

private:
    // No se si sería mejor declararlo como const, así no hay que hacer strdup ni free creo
    int pin;
};

#endif