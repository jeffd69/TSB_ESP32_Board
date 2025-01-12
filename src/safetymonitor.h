#pragma once
#include "AlpacaSafetyMonitor.h"

extern uint8_t _safemon_inputs;

class SafetyMonitor : public AlpacaSafetyMonitor {
private:
    static uint8_t _n_safetymonitors;
    static SafetyMonitor *_safetymonitor_array[2];
    uint8_t _safetymonitor_index;

    bool _is_safe;  // overall safety status
    uint16_t rain_delay, power_delay;

    // alpaca json
    void aReadJson(JsonObject &root);
    void aWriteJson(JsonObject &root);

    // alpaca getters
    void aGet_IsSafe(AsyncWebServerRequest *request)  {  _alpacaServer->respond(request,  _is_safe);  }

public:
    SafetyMonitor();
    bool Begin();
    bool Loop();
    bool is_connected();
};