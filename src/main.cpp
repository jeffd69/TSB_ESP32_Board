#include "main.h"

Dome domeDevice;
Switch switchDevice;
SafetyMonitor safemonDevice;

WiFiServer tcpServer(TCP_PORT);
WiFiClient tcpClient;

AlpacaServer alpacaServer("Alpaca_ESP32");

uint16_t _shift_reg_in, _shift_reg_out, _prev_shift_reg_out;
bool _dome_open_button, _dome_close_button, _dome_opened_switch, _dome_closed_switch;
bool _dome_roof_open, _dome_roof_close;

uint8_t _safemon_inputs;

uint8_t _sw_in[8], _sw_out[8];
uint8_t _sw_pwm[4];
uint8_t _sw_pwm_pins[4] = {SR_OUT_PWM0, SR_OUT_PWM1, SR_OUT_PWM2, SR_OUT_PWM3};

uint32_t tmr_LED, tmr_shreg;

uint16_t read_shift_register( void );
void write_shift_register( uint16_t value );
void init_IO( void );

void setup() {
  // setup serial
  Serial.begin(115200, SERIAL_8N1);
  delay(100);

  setup_wifi();

  // setup ASCOM Alpaca server
  alpacaServer.begin(ALPACA_UDP_PORT, ALPACA_TCP_PORT);
  //alpacaServer.debug;   // uncoment to get Server messages in Serial monitor

  // add devices
  domeDevice.Begin();
  alpacaServer.addDevice(&domeDevice);

  safemonDevice.Begin();
  alpacaServer.addDevice(&safemonDevice);

  switchDevice.Begin();
  alpacaServer.addDevice(&switchDevice);
  
  // load settings
  alpacaServer.loadSettings();
}
  
void loop() {
  
  domeDevice.Loop();
  safemonDevice.Loop();
  switchDevice.Loop();

  delay(1); 
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
