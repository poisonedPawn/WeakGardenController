#include <Arduino.h>
#include "Configuration.h"
#include "MyMQTTtools.h"
//#include "ESP8266tools.h"
//#include <PubSubClient.h>
//#include "BMEtools.h"
#include "SoilMoisture.h"
#include <BME280I2C.h>
#include <Wire.h>
#include <SPI.h>

BME280I2C bme;
SoilMoisture smSensor1;
//Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
float temperature, humidity, pressure, altitude;

MyMQTTtools myMQTTtools;

long probeTime = 30000;    // 3min = 180000, 30seg = 30000
long lastMsg = -probeTime; // Cycle porpouses
char msg[BUFFER_SIZE];
int value = 0;
int sensorValue = -1;

void bme280setup()
{
  Wire.begin(); // Necesario para BME

  bme.begin();
  switch (bme.chipModel())
  {
  case BME280::ChipModel_BME280:
    Serial.println("Found BME280 sensor! Success.");
    break;
  case BME280::ChipModel_BMP280:
    Serial.println("Found BMP280 sensor! No Humidity available.");
    break;
  default:
    Serial.println("Found UNKNOWN sensor! Error!");
  }
}

void setup()
{
  Serial.println("\nSetup Inicio");
  myMQTTtools.setup();
  bme280setup();
  // Señalización led -> fin del setup
  digitalWrite(LED, LOW);
  delay(1000);
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  Serial.println("\nSetup Fin");
}

void loop()
{
  myMQTTtools.keepalive();

  long now = millis();
  if (now - lastMsg > probeTime)
  {
    

    float temp(NAN), hum(NAN), pres(NAN);
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);

    lastMsg = now;
    ++value;

    bme.read(pres, temp, hum, tempUnit, presUnit);
    snprintf(msg, BUFFER_SIZE, "%f", temp);
    Serial.print("Temperatura: ");
    Serial.print(temp);
    Serial.print(" Temperatura parse: ");
    Serial.println(msg);
    myMQTTtools.publish(MQTT_TOPICT, msg);

    snprintf(msg, BUFFER_SIZE, "%f", hum);
    Serial.print("Humedad: ");
    Serial.println(hum);
    Serial.print(" Humedad parse: ");
    Serial.println(msg);
    myMQTTtools.publish(MQTT_TOPICH, msg);

    snprintf(msg, BUFFER_SIZE, "%f", pres);
    Serial.print("Presion: ");
    Serial.println(pres);
    Serial.print(" Presion parse: ");
    Serial.println(msg);
    myMQTTtools.publish(MQTT_TOPICP, msg);

    sensorValue = smSensor1.read();
    snprintf(msg, BUFFER_SIZE, "%d", sensorValue);
    Serial.print("Humedad Suelo: ");
    Serial.println(sensorValue);
    Serial.print("Humedad Suelo parse: ");
    Serial.println(msg);
    myMQTTtools.publish(MQTT_TOPICHS, msg);
  }

  Serial.println("Loop");
  delay(5000);
}