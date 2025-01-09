#pragma once
#include <Arduino.h>
#include "config.h"
#include "pins.h"

#include <ESP_WiFiManager.h>
//#include <ESP_DoubleResetDetector.h>
#include <ESPmDNS.h>

#include "AlpacaServer.h"
#include "dome.h"
#include "switch.h"
#include "safetymonitor.h"

// weather sensors loop delay
static unsigned long measureDelay = 5000;   // Sensors read cycle in ms. Always greater than 3000
static unsigned long lastTimeRan;           // a

// module setup
void setup_wifi();

