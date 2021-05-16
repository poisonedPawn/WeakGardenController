#include "Configuration.hpp"
//#include "BME280tools.h"
#include "SoilMoisture.hpp"
#include <BME280I2C.h>
#include <Wire.h>
#include <SPI.h>
#include "MyMQTTtools.hpp"
#include "ESP8266_Utils_OTA.hpp"

BME280I2C bme;

MyMQTTtools myMQTTtools;

long probeTime = 180000;   // 3min = 180000, 30seg = 30000
long lastMsg = -probeTime; // Cycle porpouses
char mqtt_msg_buffer[BUFFER_SIZE];
int deepSleepTime = 180; // En segundos
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
  InitOTA();
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
  myMQTTtools.publish("/pnetwork/debug", "Estoy funcionando");
  long now = millis();
  if (now - lastMsg > probeTime)
  {
    lastMsg = now;
    float temp(NAN), hum(NAN), pres(NAN);
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);
    
    bme.read(pres, temp, hum, tempUnit, presUnit);
    snprintf(mqtt_msg_buffer, BUFFER_SIZE, "%f", temp);
    //Serial.print("Temperatura: ");
    //Serial.println(temp);
    myMQTTtools.publish(MQTT_TOPICT, mqtt_msg_buffer);

    snprintf(mqtt_msg_buffer, BUFFER_SIZE, "%f", hum);
    //Serial.print("Humedad: ");
    //Serial.println(hum);
    myMQTTtools.publish(MQTT_TOPICH, mqtt_msg_buffer);

    snprintf(mqtt_msg_buffer, BUFFER_SIZE, "%f", pres);
    //Serial.print("Presion: ");
    //Serial.println(pres);
    myMQTTtools.publish(MQTT_TOPICP, mqtt_msg_buffer);

    soilMoistureValue = smSensor1.read();
    snprintf(mqtt_msg_buffer, BUFFER_SIZE, "%d", soilMoistureValue);
    //Serial.print("Humedad Suelo: ");
    //Serial.println(soilMoistureValue);
    myMQTTtools.publish(MQTT_TOPICHS, mqtt_msg_buffer);
  }

  /* El delay despues de esto es para que haga bien el update, 
  * tengo que encontrar una forma asincrona de hacer esto:
  * 
  * PROBLEMA: Para actualizar o no se hace deepSleep o se hace de forma asincrona, 
  *  si no, falla la actualización y se corrompe.
  * 
  * SOLUCIONES: 
  * - Actualización por web, se sube el fichero o algo así.
  * - Actualización automática mediante git??
  */
  ArduinoOTA.handle();
  delay(60000);

  Serial.println("Me Voy a dormir");
  delay(15);
  ESP.deepSleep((deepSleepTime * 1000000)); //Go to deep sleep for ever
}