/**************************************************************************************************
  Filename:       Switch.cpp
  Revised:        Date: 2024-12-02
  Revision:       Revision: 01

  Description:    ASCOM Alpaca ESP32 TSBoard implementation
**************************************************************************************************/
#include "Switch.h"

const uint32_t _num_of_switch_devices = 20;

SwitchDevice_t init_switch_device[_num_of_switch_devices] = {
    {false, "Switch 0", "Read-only input 1", 0, 0, 1, 0},
    {false, "Switch 1", "Read-only input 2", 0, 0, 1, 0},
    {false, "Switch 2", "Read-only input 3", 0, 0, 1, 0},
    {false, "Switch 3", "Read-only input 4", 0, 0, 1, 0},
    {false, "Switch 4", "Read-only input 5", 0, 0, 1, 0},
    {false, "Switch 5", "Read-only input 6", 0, 0, 1, 0},
    {false, "Switch 6", "Read-only input 7", 0, 0, 1, 0},
    {false, "Switch 7", "Read-only input 8", 0, 0, 1, 0},

    {true, "Switch 8", "Read-write output 1", 0, 0, 1, 0},
    {true, "Switch 9", "Read-write output 2", 0, 0, 1, 0},
    {true, "Switch 10", "Read-write output 3", 0, 0, 1, 0},
    {true, "Switch 11", "Read-write output 4", 0, 0, 1, 0},
    {true, "Switch 12", "Read-write output 5", 0, 0, 1, 0},
    {true, "Switch 13", "Read-write output 6", 0, 0, 1, 0},
    {true, "Switch 14", "Read-write output 7", 0, 0, 1, 0},
    {true, "Switch 15", "Read-write output 8", 0, 0, 1, 0},

    {true, "Switch 16", "PWM output 1", 0, 0, 100, 0},
    {true, "Switch 17", "PWM output 2", 0, 0, 100, 0},
    {true, "Switch 18", "PWM output 3", 0, 0, 100, 0},
    {true, "Switch 19", "PWM output 4", 0, 0, 100, 0}
    };

Switch::Switch() : AlpacaSwitch()
{
  //_p_swtc = AlpacaSwitch::_p_switch_devices;
}

bool Switch::is_connected() {
  return AlpacaSwitch::_isconnected;
}

void Switch::Begin()
{

}

void Switch::Loop()
{
  // copy inputs to AlpacaSwitch::_p_switch_devices
  for(int i=0; i<8; i++)
  {
    if(_sw_in[i] == 1)                                          // set input value to AlpacaSwitch::_p_switch_devices
      AlpacaSwitch::_p_switch_devices[i].value = 1;
    else
      AlpacaSwitch::_p_switch_devices[i].value = 0;
  
    if( AlpacaSwitch::_p_switch_devices[i + 8].value == 1 )     // if value of AlpacaSwitch::_p_switch_devices == 1, set relay
      _sw_out[i] = 1;
    else
      _sw_out[i] = 0;
  }

  for(int i=0; i<4; i++)
  {
    _sw_pwm[i] = AlpacaSwitch::_p_switch_devices[i + 16].value;
  }
}

/**
 * This methode is called by AlpacaSwitch to manipulate physical device
 */
bool Switch::_writeSwitchValue(uint32_t id, uint32_t value)
{
  if((id < 8) || (id > (_num_of_switch_devices-1)))
    return false;

  if((id > 7 ) && ( id < 16 ))
    _sw_out[id - 8] = (value != 0 ? true : false);
  else
    _sw_pwm[id - 16] = (uint8_t)value;

  return true;
}


void Switch::AlpacaReadJson(JsonObject &root)
{
  AlpacaSwitch::aReadJson(root);
  char sw_name[aSwitchNameSize];

  if (JsonObject obj_config = root["Configuration"])
  {
    for(int i=0; i<_num_of_switch_devices; i++ )
    {
      sprintf(sw_name, "Switch_%i", i);
      // InitSwitchName(i, obj_config["Name"] | GetSwitchName(i));
      // void InitSwitchName(uint32_t id, const char* name) { strlcpy(_p_switch_devices[id].name, name, kSwitchNameSize ); };

      obj_config[sw_name] | AlpacaSwitch::_p_switch_devices[i].name;
    }

  }
}

void Switch::AlpacaWriteJson(JsonObject &root)
{
  int _min, _max, _stp, _val;

  AlpacaSwitch::aWriteJson(root);
  char sw_name[aSwitchNameSize];
  char sw_state[aSwitchDescriptionSize];

  // Config
  JsonObject obj_config = root["Configuration"].to<JsonObject>();
  for(int i=0; i<_num_of_switch_devices; i++ )
  {
    sprintf(sw_name, "Switch_%i", i);
    obj_config[sw_name] | AlpacaSwitch::_p_switch_devices[i].name;
  }

  // #add # for read only
  JsonObject obj_states = root["#States"].to<JsonObject>();
  for(int i=0; i<_num_of_switch_devices; i++ )
  {
    _min = AlpacaSwitch::_p_switch_devices[i].min_value;
    _max = AlpacaSwitch::_p_switch_devices[i].max_value;
    _stp = AlpacaSwitch::_p_switch_devices[i].step;
    _val = AlpacaSwitch::_p_switch_devices[i].value;
    sprintf(sw_name, "Switch_%i", i);
    sprintf(sw_state, "%s, %s, min %i, max %i, step %i, value %i", AlpacaSwitch::_p_switch_devices[i].name, 
      (AlpacaSwitch::_p_switch_devices[i].can_write ? "RW" : "R"), _min, _max, _stp, _val);
    obj_states[sw_name] = sw_state;
  }
}


