/**************************************************************************************************
  Filename:       dome.cpp
  Revised:        Date: 2024-12-02
  Revision:       Revision: 01

  Description:    Dome Device implementation
**************************************************************************************************/
#include "dome.h"

Dome::Dome() : AlpacaDome()
{
	// constructor
}

void Dome::Begin()
{
	// init shutter status
	if( _use_switch )
	{
		if( _dome_switch_closed )
			_shutter = AlpacaShutterStatus_t::aClosed;
		else if( _dome_switch_opened )
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
			_dome_relay_close = false;							// turn relays OFF
			_dome_relay_open = false;
			return;
		}

		if(( _shutter == AlpacaShutterStatus_t::aOpening ) && ( _dome_switch_opened ))
		{
			_shutter == AlpacaShutterStatus_t::aOpen;
			_slew = false;
			_dome_relay_close = false;		// turn relays OFF
			_dome_relay_open = false;
		}
		
		if(( _shutter == AlpacaShutterStatus_t::aClosing ) && ( _dome_switch_closed ))
		{
			_shutter == AlpacaShutterStatus_t::aClosed;
			_slew = false;
			_dome_relay_close = false;		// turn relays OFF
			_dome_relay_open = false;
		}
	}
	else
	{
		if(( _shutter == AlpacaShutterStatus_t::aOpening ) && ( millis() > _timer_end ))
		{
			_shutter == AlpacaShutterStatus_t::aOpen;
			_slew = false;
			_dome_relay_close = false;		// turn relays OFF
			_dome_relay_open = false;
		}
		
		if(( _shutter == AlpacaShutterStatus_t::aClosing ) && ( millis() > _timer_end ))
		{
			_shutter == AlpacaShutterStatus_t::aClosed;
			_slew = false;
			_dome_relay_close = false;		// turn relays OFF
			_dome_relay_open = false;
		}
	}
}

bool Dome::is_connected() {
	return AlpacaDome::_isconnected;
}

const bool Dome::_putAbort()	// stops shutter motor, sets _shutter to error, set _slew to false
{
    _shutter = AlpacaShutterStatus_t::aError;
    _slew = false;
	
	_timer_ini = 0;
	_timer_end = 0;

	_dome_relay_close = false;		// turn relays OFF
	_dome_relay_open = false;
	
	return true;
}

const bool Dome::_putClose()
{
    if( _shutter == AlpacaShutterStatus_t::aOpening )
	{
		return false;
	}
	else if( _shutter == AlpacaShutterStatus_t::aClosing )
	{
		return true;
	}
	else
	{
		_slew = true;
		_shutter = AlpacaShutterStatus_t::aClosing;
		
		_timer_ini = millis();
		_timer_end = _timer_ini + _timeout * 1000;

		_dome_relay_close = true;		// turn close relays ON
		_dome_relay_open = false;		// turn open relays OFF
	}
	
	return true;
}

const bool Dome::_putOpen()
{
    if( _shutter == AlpacaShutterStatus_t::aClosing )
	{
		return false;
	}
	else if( _shutter == AlpacaShutterStatus_t::aOpening )
	{
		return true;
	}
	else
	{
		_slew = true;
		_shutter = AlpacaShutterStatus_t::aOpening;
		
		_timer_ini = millis();
		_timer_end = _timer_ini + _timeout * 1000;

		_dome_relay_close = false;		// turn close relays OFF
		_dome_relay_open = true;			// turn open relays ON
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

	if (JsonObject obj_config = root["Configuration"])
	{
		bool _us = obj_config["Use_limit_switches"] | _use_switch;
		int32_t _to = obj_config["Shutter_timeout"] | _timeout;

		_use_switch = _us;
		_timeout = _to;

	}
}

void Dome::AlpacaWriteJson(JsonObject &root)
{
    AlpacaDome::aWriteJson(root);

    // Config
    JsonObject obj_config = root["Configuration"].to<JsonObject>();
    obj_config["Use_limit_switches"] = _use_switch;
    obj_config["Shutter_timeout"] = _timeout;

    // #add # for read only
    JsonObject obj_states = root["#States"].to<JsonObject>();
    obj_states["Shutter"] = AlpacaDome::aShutterStatusStr[(int)_shutter];
	obj_states["Use_limit_switches"] = (_use_switch ? "true" : "false");
    obj_states["Shutter_timeout"] = _timeout;
}

