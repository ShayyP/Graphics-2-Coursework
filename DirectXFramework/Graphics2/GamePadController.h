#pragma once
#include "Core.h"
#include "DirectXCore.h"
#include <XInput.h>
#pragma comment(lib, "XInput.lib")

class GamePadController
{
public:
	GamePadController();
	~GamePadController();
	void ProcessGameController();

	inline SHORT GetThumbRX() { return _thumbRX; }
	inline SHORT GetThumbRY() { return _thumbRY; }
	inline SHORT GetThumbLX() { return _thumbLX; }
	inline SHORT GetThumbLY() { return _thumbLY; }

private:
	XINPUT_STATE _controllerState;
	DWORD		 _lastPacketNumber;
	bool		 _firstTime;

	// These two values are used to avoid having to calculate square roots (which are very time consuming)
	// when we are checking if the movement of the left or right thumb stick is in the dead zone
	DWORD		_leftThumbDeadZoneSquared;
	DWORD		_rightThumbDeadZoneSquared;

	SHORT _thumbRX;
	SHORT _thumbRY;
	SHORT _thumbLX;
	SHORT _thumbLY;
};

