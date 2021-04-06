#include "Configuration.h"
#include "MyMQTTtools.h"
//#include "BME280tools.h"
#include "SoilMoisture.h"
#include <BME280I2C.h>
#include <Wire.h>
#include <SPI.h>

BME280I2C bme;

MyMQTTtools myMQTTtools;

long probeTime = 30000;    // 3min = 180000, 30seg = 30000
long lastMsg = -probeTime; // Cycle porpouses
char mqtt_msg_buffer[BUFFER_SIZE];

SoilMoisture smSensor1;
int soilMoistureValue = -1;

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

    bme.read(pres, temp, hum, tempUnit, presUnit);
    snprintf(mqtt_msg_buffer, strlen(mqtt_msg_buffer), "%f", temp);
    Serial.print("Temperatura: ");
    Serial.println(temp);
    myMQTTtools.publish(MQTT_TOPICT, mqtt_msg_buffer);

    snprintf(mqtt_msg_buffer, strlen(mqtt_msg_buffer), "%f", hum);
    Serial.print("Humedad: ");
    Serial.println(hum);
    myMQTTtools.publish(MQTT_TOPICH, mqtt_msg_buffer);

    snprintf(mqtt_msg_buffer, strlen(mqtt_msg_buffer), "%f", pres);
    Serial.print("Presion: ");
    Serial.println(pres);
    myMQTTtools.publish(MQTT_TOPICP, mqtt_msg_buffer);

    soilMoistureValue = smSensor1.read();
    snprintf(mqtt_msg_buffer, strlen(mqtt_msg_buffer), "%d", soilMoistureValue);
    Serial.print("Humedad Suelo: ");
    Serial.println(soilMoistureValue);
    myMQTTtools.publish(MQTT_TOPICHS, mqtt_msg_buffer);
  }
  Serial.println("Loop");
  delay(probeTime - 1);
}