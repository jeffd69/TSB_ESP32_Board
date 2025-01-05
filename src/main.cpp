#include "main.h"

SafetyMonitor safetymonitor;
ObservingConditions observingconditions;

WiFiServer tcpServer(TCP_PORT);
WiFiClient tcpClient;

AlpacaServer alpacaServer("Alpaca_ESP32");

void setup() {
  // setup serial
  Serial.begin(115200, SERIAL_8N1);

  setup_wifi();

  // setup ASCOM Alpaca server
  alpacaServer.begin(ALPACA_UDP_PORT, ALPACA_TCP_PORT);
  //alpacaServer.debug;   // uncoment to get Server messages in Serial monitor

  // add devices
  safetymonitor.begin();
  alpacaServer.addDevice(&safetymonitor);

  observingconditions.begin();
  alpacaServer.addDevice(&observingconditions);
  
  // load settings
  alpacaServer.loadSettings();
  meteo1.setup_i2cmlxbme();
}
  
void loop() {
  if (millis() > lastTimeRan + measureDelay)  {   // read every measureDelay without blocking Webserver
    meteo1.update_i2cmlxbme(measureDelay);
    safetymonitor.update(meteo1,measureDelay);
    observingconditions.update(meteo1, measureDelay);
    lastTimeRan = millis();
  }
  delay(50); 
}

void setup_wifi()
{
  pinMode(PIN_WIFI_LED, OUTPUT);

  // setup wifi
  Serial.print(F("\n# Starting WiFi"));

  //DoubleResetDetector drd = DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);
  ESP_WiFiManager ESP_wifiManager(HOSTNAME);
  ESP_wifiManager.setConnectTimeout(30);

  //if (ESP_wifiManager.WiFi_SSID() == "" || drd.detectDoubleReset()) {
  if (ESP_wifiManager.WiFi_SSID() == "" ) {
    Serial.println(F("# Starting Config Portal"));
    //digitalWrite(PIN_WIFI_LED, HIGH);
    if (!ESP_wifiManager.startConfigPortal()) {
      Serial.println(F("# Not connected to WiFi"));
    } else {
      Serial.println(F("# connected"));
    }
  } else {
    WiFi.mode(WIFI_STA);
    WiFi.begin();
  } 
  WiFi.waitForConnectResult();
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("# Failed to connect"));
  }
  else {
    Serial.print(F("# Local IP: "));
    Serial.println(WiFi.localIP());
    //digitalWrite(PIN_WIFI_LED, HIGH);
    if(!MDNS.begin("HOSTNAME")) {
     Serial.println("# Error starting mDNS");
     return;
    }
  }
}
