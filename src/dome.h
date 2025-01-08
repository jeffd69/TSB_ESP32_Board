/**************************************************************************************************
  Filename:       dome.h
  Revised:        $Date: 2024-12-02$
  Revision:       $Revision: 01 $
  Description:    Device Alpaca Dome
**************************************************************************************************/
#pragma once
#include "AlpacaDome.h"

extern bool _dome_open_button, _dome_close_button, _dome_switch_opened, _dome_switch_closed;
extern bool _dome_relay_open, _dome_relay_close;

class Dome : public AlpacaDome
{
private:
	AlpacaShutterStatus_t _shutter;		// shutter status
	bool _slew;							// true when shutter is moving
	bool _use_switch;					// if true, use limit switches, else use timeout
	int32_t _timeout;					// open/close timeout
	int32_t _timer_ini;					// timer init of movement
	int32_t _timer_end;					// timer init of movement

	const bool _putAbort();				// to be implemented here
	const bool _putClose();
	const bool _putOpen();
	const AlpacaShutterStatus_t _getShutter();
	const bool _getSlewing();
	void AlpacaReadJson(JsonObject &root);
	void AlpacaWriteJson(JsonObject &root);

protected:

public:
	Dome();
	void Begin();
	void Loop();
	bool is_connected();
};