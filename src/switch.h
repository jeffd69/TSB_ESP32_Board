/**************************************************************************************************
  Filename:       switch.h
  Revised:        Date: 2024-12-02
  Revision:       Revision: 01

  Description:    ASCOM Alpaca ESP32 TSBoard implementation
**************************************************************************************************/
#pragma once
#include "AlpacaSwitch.h"

// comment/uncomment to enable/disable debugging
// #define DEBUG_SWITCH

extern uint8_t _sw_in[8], _sw_out[8];
extern uint8_t _sw_pwm[4];

class Switch : public AlpacaSwitch
{
private:
    static uint8_t _n_switches;
    static Switch *_switch_array[2];
    uint8_t _switch_index;

    bool _writeSwitchValue(uint32_t id, uint32_t value);

    void AlpacaReadJson(JsonObject &root);
    void AlpacaWriteJson(JsonObject &root);

public:
    Switch();
    bool Begin();
    void Loop();
    bool is_connected();
};