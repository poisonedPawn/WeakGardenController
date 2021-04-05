#include "SoilMoisture.h"
#include "Configuration.h"
SoilMoisture::SoilMoisture()
{
    pin = MOISTURES;
    //_ssid= strdup(WIFI_SSID);
    //_password=strdup(WIFI_PASSWORD);
}

SoilMoisture::~SoilMoisture()
{
    pin = 0;
    //free(_ssid);
    //free(_password);
}

int SoilMoisture::read()
{
    int sensorValue = analogRead(MOISTURES);
    return sensorValue;
}