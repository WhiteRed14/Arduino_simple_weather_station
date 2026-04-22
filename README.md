# Arduino Simple Weather Station

An Arduino-based weather station that reads environmental data from multiple sensors and uploads it to [ThingSpeak](https://thingspeak.com) over Wi-Fi via an ESP8266 module.

This project is co-created and was originally developed to work alongside the **IoT Weather Station Android** app:
[https://github.com/kwerulik/IoT-Weather-Station-Android](https://github.com/kwerulik/IoT-Weather-Station-Android)

---

## Hardware

| Component | Purpose |
|---|---|
| Arduino (Uno / Nano) | Main microcontroller |
| DHT22 | Temperature & humidity |
| BMP180 | Temperature & barometric pressure |
| MQ-135 | Air quality (analog) |
| ESP8266 (AT firmware) | Wi-Fi connectivity |
| 7-segment display (single digit) | Status indicator |

The circuit schematic and breadboard layout are provided as Fritzing files (`.fzz` / `.fzpz`) in this repository.

---

## Sensors & Data Fields

| ThingSpeak Field | Sensor | Measurement |
|---|---|---|
| Field 1 | DHT22 + BMP180 (averaged) | Temperature (°C) |
| Field 2 | DHT22 | Relative humidity (%) |
| Field 3 | MQ-135 | Air quality (raw analog) |
| Field 4 | BMP180 | Barometric pressure (hPa) |

---

## 7-Segment Display States

| Symbol | Meaning |
|---|---|
| `0` | Startup / initializing |
| `1` (blinking) | Connecting to Wi-Fi |
| `2` | Idle / connected |
| `3` | Sending data to ThingSpeak |
| `b` | BMP180 error (program halts) |
| `d` | DHT22 read error (retries) |

---

## Configuration

Before uploading, edit the following constants at the top of `Arduino_weather_station_code.ino`:

```cpp
const String ssid     = "WIFI SSID";
const String password = "WIFI PASSWORD";
const String apiKey   = "API KEY";         // ThingSpeak Write API Key
const String host     = "api.thingspeak.com";
```

---

## Dependencies

Install the following libraries via the Arduino Library Manager:

- [Adafruit BMP085/BMP180](https://github.com/adafruit/Adafruit-BMP085-Library)
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)
- Adafruit Unified Sensor

---

## Files

| File | Description |
|---|---|
| `Arduino_weather_station_code.ino` | Main Arduino sketch |
| `Arduino simple weather station sketch.fzz` | Fritzing full circuit diagram |
| `DHT22-vcc-dta-gnd-module.fzpz` | Custom Fritzing part — DHT22 module |
| `MQ-135 Sensor Module.fzpz` | Custom Fritzing part — MQ-135 module |
