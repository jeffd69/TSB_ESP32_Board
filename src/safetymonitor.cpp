#include "safetymonitor.h"

SafetyMonitor::SafetyMonitor() : AlpacaSafetyMonitor()
{
    _is_safe = true;
}

bool SafetyMonitor::Begin()
{
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

    JsonObject obj_state  = root["State"].to<JsonObject>();
    obj_state["Rain_delay"] = rain_delay;
    obj_state["Power_delay"] = power_delay;
    obj_state["Is_safe"] = _is_safe;
}

