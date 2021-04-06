# Weak Garden Controller

This is a garden controller designed for nodeMCU dev board. Implements MQTT/TCP client. It is configured through a header C file.

<img src="docs/controller2.jpg" width="200">

## Features
1. PlatformIO: simple development, build and uploap 
2. Arduino Framework
3. NodeMCU - ESP8266-12E
4. Wifi
5. MQTT
    1. 3 topics for BME
    2. 1 topic for MoistureSoil sensor
6. BME280 [SPI-MODE] (Presure, Humidity, Temperature) and MoistureSoil [AnalogRead]
7. Status led.
8. Jumper for RST mode.

## Roadmap

1. OTA Update
2. DeepSleep and eco function
3. Sensor Module
4. Simple config method: UI for example.
5. 3D Print Case
