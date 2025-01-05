#pragma once
// General
#define DEVICENAME              "TS Board Alpaca"
#define VERSION                 "v1.0.0"
#define COPYRIGHT               "2025 Tecnosky.eu"
#define DEBUG

// WiFi config
#define HOSTNAME                "alpaca_ts_net"
#define TCP_PORT                3117
//#define ESP_DRD_USE_EEPROM    false //true
//#define DRD_TIMEOUT           3
//#define DRD_ADDRESS           0
//#define ESP_DRD_USE_SPIFFS    false //true

// ASCOM Alpaca
#define ALPACA_UDP_PORT         32227
#define ALPACA_TCP_PORT         80

// MLX90614 sensor
#define SDApin 22               // Pins at SDA=22 SCL=21. Change them as required
#define SCLpin 21
#define bme_i2caddress 0x76




