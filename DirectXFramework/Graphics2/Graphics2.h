#pragma once

#include "Cube.h"
#include "TerrainNode.h"
#include "MoveableNode.h"
#include "SkyNode.h"

class Graphics2 : public DirectXFramework
{
public:
	enum InputMode{ Keyboard, Controller };
	enum Turned { None, Left, Right };
	void CreateSceneGraph();
	void UpdateSceneGraph();
	void HandleKeyboardInput();
	void HandleControllerInput();
private:
	float _rotation = 0;
	wchar_t* _brickTexture = L"Textures/Bricks.png";
	wchar_t* _woodTexture = L"Textures/Wood.png";
	wchar_t* _concreteTexture = L"Textures/Concrete.png";
	wchar_t* _woodblockTexture = L"Textures/woodblock.bmp";
	float _keyboardSpeedModifier = 3.0f;
	float _cameraSpeedLimiter = 10000.0f;
	float _mouseSpeedLimiter = 10.0f;
	InputMode _inputMode;
	Turned _turned;
	float _roll = 0;
	shared_ptr<TerrainNode> _terrain;
	shared_ptr<MoveableNode> _plane;
	bool _freeCam = false;
	bool _freeCamPressed = false;
};

