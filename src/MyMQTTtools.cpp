#include "MyMQTTtools.hpp"

ESP8266tools myESPtools;
WiFiClient _WiFiclient;
PubSubClient MQTTClient(_WiFiclient);

MyMQTTtools::MyMQTTtools()
{
  Serial.begin(SERIAL_BAUD);
  while (!Serial)
  {
  } // Wait
  _ssid = WIFI_SSID;
  _password = WIFI_PASSWORD;
  _user = MQTT_USER;
  _mqtt_password = MQTT_PASSWORD;
  _mqtt_server = MQTT_SERVER;
  _mqtt_port = MQTT_PORT;
}

MyMQTTtools::~MyMQTTtools()
{
  _ssid = "";
  _password = "";
  _user = "";
  _mqtt_password = "";
  _mqtt_server = "";
  _mqtt_port = 0;
}

// MQTT Callback Function
void MyMQTTtools::callback(char *topic, byte *payload, int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1')
  {
    digitalWrite(LED, LOW); // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  }
  else
  {
    digitalWrite(LED, HIGH); // Turn the LED off by making the voltage HIGH
  }
}

// Reconnect MQTT
void MyMQTTtools::reconnect()
{
  // Loop until we're reconnected
  while (!MQTTClient.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (MQTTClient.connect("ESP8266Client"))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      MQTTClient.publish("outTopic", "hello world");
      // ... and resubscribe
      MQTTClient.subscribe("inTopic");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(MQTTClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void MyMQTTtools::keepalive()
{
  if (!_WiFiclient.connected())
  {
    reconnect();
  }
  MQTTClient.loop();

  delay(1);
}

void MyMQTTtools::setup()
{
  myESPtools.setup_wifi();
  MQTTClient.setServer(_mqtt_server, _mqtt_port);
  MQTTClient.setCallback([this](char *topic, byte *payload, unsigned int length) { this->callback(topic, payload, length); });
}

void MyMQTTtools::publish(const char *topic, String message)
{
  MQTTClient.publish(topic, message.c_str());
  delay(15);
}

void MyMQTTtools::disconnectWifi()
{
  myESPtools.disconnectWifi();
}