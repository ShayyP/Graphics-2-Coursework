#include "GamePadController.h"

GamePadController::GamePadController(void)
{
	_firstTime = true;
	_leftThumbDeadZoneSquared = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	_rightThumbDeadZoneSquared = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE * XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
}

GamePadController::~GamePadController(void)
{
}

char* GamePadController::ProcessGameController()
{
	DWORD magnitudeSquared;

	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));
	// Get the state of the controller
    if (XInputGetState(0, &_controllerState) != ERROR_SUCCESS)
	{
		// Controller is not connected or is lost
		return "";
	}
	// Uncomment out this if statement if you only want to test the controller if it has
	// changed since the last time it was tested
	//if (!_firstTime && _controllerState.dwPacketNumber != _lastPacketNumber)
	//{
		// No change in controller states
	//}
	_firstTime = false;

	// Deal with the left thumb stick
	_thumbLX = _controllerState.Gamepad.sThumbLX;
	_thumbLY = _controllerState.Gamepad.sThumbLY;

	// Determine how far the controller is pushed
	magnitudeSquared = _thumbLX * _thumbLX + _thumbLY * _thumbLY;

	// check if the controller is inside a circular dead zone. We do it this way to avoid having to
	// take the square root of the magnitude above.
	if (magnitudeSquared <= _leftThumbDeadZoneSquared)
	{
		_thumbLX = 0;
		_thumbLY = 0;
	}

	// Deal with the right thumb stick
	_thumbRX = _controllerState.Gamepad.sThumbRX;
	_thumbRY = _controllerState.Gamepad.sThumbRY;

	// Determine how far the controller is pushed
	magnitudeSquared = _thumbRX * _thumbRX + _thumbRY * _thumbRY;

	//check if the controller is inside a circular dead zone
	if (magnitudeSquared <= _rightThumbDeadZoneSquared)
	{
		_thumbRX = 0;
		_thumbRY = 0;
	}

	// Test the different digital buttons
	WORD buttons = _controllerState.Gamepad.wButtons;
	if (buttons & XINPUT_GAMEPAD_A)
	{
		return "A";
	}
	if (buttons & XINPUT_GAMEPAD_X)
	{
		return "X";
	}
	if (buttons & XINPUT_GAMEPAD_Y)
	{
		return "Y";
	}

	// Check right trigger
	BYTE rightTrigger = _controllerState.Gamepad.bRightTrigger;
	if (rightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		rightTrigger = 0;
		return "RT";
	}

	// Other button mask values that can be used are:
	//
	//  XINPUT_GAMEPAD_START, 
	//  XINPUT_GAMEPAD_BACK, 
	//  XINPUT_GAMEPAD_LEFT_THUMB, 
	//  XINPUT_GAMEPAD_RIGHT_THUMB, 
	//  XINPUT_GAMEPAD_LEFT_SHOULDER, 
	//  XINPUT_GAMEPAD_RIGHT_SHOULDER, 
	//  XINPUT_GAMEPAD_A, 
	//  XINPUT_GAMEPAD_B, 
	//  XINPUT_GAMEPAD_X, 
	//  XINPUT_GAMEPAD_Y 

	return "";
}
