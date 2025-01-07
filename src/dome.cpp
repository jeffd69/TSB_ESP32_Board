/**************************************************************************************************
  Filename:       dome.cpp
  Revised:        Date: 2024-12-02
  Revision:       Revision: 01

  Description:    Dome Device implementation
**************************************************************************************************/
#include "Dome.h"

//const char *const Dome::aShutterStatusStr[5] = {"OPEN", "CLOSED", "OPENING", "CLOSING", "ERROR"};

Dome::Dome() : AlpacaDome()
{
	// constructor
}

void Dome::Begin()
{
	// init shutter status
	if( _use_switch )
	{
		if( _dome_closed_switch )
			_shutter = AlpacaShutterStatus_t::aClosed;
		else if( _dome_opened_switch )
			_shutter = AlpacaShutterStatus_t::aOpen;
		else
			_shutter = AlpacaShutterStatus_t::aError;
	}
	else
	{
		_shutter = AlpacaShutterStatus_t::aError;
	}
}

void Dome::Loop()
{
    // simulation of roof moving and changing status using timer (no limit switches)

	if( _use_switch )
	{
		if(( millis() - _timer_ini ) > (_timeout * 1000 ))		// timeout !!!!!!!!!!!
		{
			_shutter = AlpacaShutterStatus_t::aError;			// set error status
			_slew = false;
			_timer_ini = 0;
			_timer_end = 0;
			_dome_roof_close = false;							// turn relays OFF
			_dome_roof_open = false;
			return;
		}

		if(( _shutter == AlpacaShutterStatus_t::aOpening ) && ( _dome_opened_switch ))
		{
			_shutter == AlpacaShutterStatus_t::aOpen;
			_slew = false;
			_dome_roof_close = false;		// turn relays OFF
			_dome_roof_open = false;
		}
		
		if(( _shutter == AlpacaShutterStatus_t::aClosing ) && ( _dome_closed_switch ))
		{
			_shutter == AlpacaShutterStatus_t::aClosed;
			_slew = false;
			_dome_roof_close = false;		// turn relays OFF
			_dome_roof_open = false;
		}
	}
	else
	{
		if(( _shutter == AlpacaShutterStatus_t::aOpening ) && ( millis() > _timer_end ))
		{
			_shutter == AlpacaShutterStatus_t::aOpen;
			_slew = false;
			_dome_roof_close = false;		// turn relays OFF
			_dome_roof_open = false;
		}
		
		if(( _shutter == AlpacaShutterStatus_t::aClosing ) && ( millis() > _timer_end ))
		{
			_shutter == AlpacaShutterStatus_t::aClosed;
			_slew = false;
			_dome_roof_close = false;		// turn relays OFF
			_dome_roof_open = false;
		}
	}
}

const bool Dome::_putAbort()	// stops shutter motor, sets _shutter to error, set _slew to false
{
    AlpacaDome::_shutter_state = AlpacaShutterStatus_t::aError;
    AlpacaDome::_slewing = false;
	
	_timer_ini = 0;
	_timer_end = 0;

	_dome_roof_close = false;		// turn relays OFF
	_dome_roof_open = false;
	
	return true;
}

const bool Dome::_putClose()
{
    if( AlpacaDome::_shutter_state == AlpacaShutterStatus_t::aOpening )
	{
		return false;
	}
	else if( AlpacaDome::_shutter_state == AlpacaShutterStatus_t::aClosing )
	{
		return true;
	}
	else
	{
		AlpacaDome::_slewing = true;
		AlpacaDome::_shutter_state = AlpacaShutterStatus_t::aClosing;
		
		_timer_ini = millis();
		_timer_end = _timer_ini + _timeout * 1000;

		_dome_roof_close = true;		// turn close relays ON
		_dome_roof_open = false;		// turn open relays OFF
	}
	
	return true;
}

const bool Dome::_putOpen()
{
    if( AlpacaDome::_shutter_state == AlpacaShutterStatus_t::aClosing )
	{
		return false;
	}
	else if( AlpacaDome::_shutter_state == AlpacaShutterStatus_t::aOpening )
	{
		return true;
	}
	else
	{
		AlpacaDome::_slewing = true;
		AlpacaDome::_shutter_state = AlpacaShutterStatus_t::aOpening;
		
		_timer_ini = millis();
		_timer_end = _timer_ini + _timeout * 1000;

		_dome_roof_close = false;		// turn close relays OFF
		_dome_roof_open = true;			// turn open relays ON
	}
	
	return true;
}

const AlpacaShutterStatus_t Dome::_getShutter()
{
    return _shutter;
}

const bool Dome::_getSlewing()
{
    return _slew;
}


void Dome::AlpacaReadJson(JsonObject &root)
{
	AlpacaDome::aReadJson(root);

	if (JsonObject obj_config = root["DomeConfiguration"])
	{
		bool _us = obj_config["Use limit switches"] | _use_switch;
		int32_t _to = obj_config["Shutter timeout"] | _timeout;
		int32_t _oc = obj_config["Extend closing"] | _overclose;

		_use_switch = _us;
		_timeout = _to;
		_overclose = _oc;

	}
}

void Dome::AlpacaWriteJson(JsonObject &root)
{
    AlpacaDome::aWriteJson(root);

    // Config
    JsonObject obj_config = root["DomeConfiguration"].to<JsonObject>();
    obj_config["Use_limit_switches"] = _use_switch;
    obj_config["Shutter_timeout"] = _timeout;
	obj_config["Extend_closing"] = _overclose;

    // #add # for read only
    JsonObject obj_states = root["#States"].to<JsonObject>();
    obj_states["Shutter"] = k_shutter_state_str[(int)_shutter];
	obj_states["Use_limit_switches"] = (_use_switch ? "true" : "false");
    obj_states["Shutter_timeout"] = _timeout;
	obj_states["Extend_closing"] = _overclose;
}

