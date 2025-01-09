#include "safetymonitor.h"

// cannot call member functions directly from interrupt, so need these helpers for up to 1 SafetyMonitor
uint8_t SafetyMonitor::_n_safetymonitors = 0;
SafetyMonitor *SafetyMonitor::_safetymonitor_array[2] = { nullptr,  nullptr };


SafetyMonitor::SafetyMonitor() : AlpacaSafetyMonitor()
{
    _safetymonitor_index = _n_safetymonitors++;
}

bool SafetyMonitor::is_connected() {
	return AlpacaSafetyMonitor::_isconnected;
}

bool SafetyMonitor::Begin()
{
    _safetymonitor_array[_safetymonitor_index] = this;
    _is_safe = true;
    return true;
}

bool SafetyMonitor::Loop()
{
    if( _safemon_inputs == 0 )
        return true;
    else
        return false;
}

void SafetyMonitor::aReadJson(JsonObject &root)
{
    AlpacaSafetyMonitor::aReadJson(root);
    if(JsonObject obj_config = root[F("Configuration")]) {
        rain_delay  = obj_config[F("Rain_delay")] | rain_delay;
        power_delay  = obj_config[F("Power_delay")] | power_delay;
    }
}

void SafetyMonitor::aWriteJson(JsonObject &root)
{
    AlpacaSafetyMonitor::aWriteJson(root);
    // read-only values marked with #
    JsonObject obj_config = root["Configuration"].to<JsonObject>();
    obj_config["Rain_delay"] = rain_delay;
    obj_config["Power_delay"] = power_delay;

    JsonObject obj_state  = root["States"].to<JsonObject>();
    obj_state["#Rain_delay"] = rain_delay;
    obj_state["#Power_delay"] = power_delay;
    obj_state["#Is_safe"] = (_is_safe ? "SAFE" : "UNSAFE");
}

