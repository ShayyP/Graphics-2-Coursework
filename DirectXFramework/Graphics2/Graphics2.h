#pragma once
#include "DirectXFramework.h"
#include "Cube.h"
#include "TerrainNode.h"

class Graphics2 : public DirectXFramework
{
public:
	enum InputMode{ Keyboard, Controller };
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
	float _keyboardSpeedModifier = 2.0f;
	float _cameraSpeedLimiter = 10000.0f;
	InputMode _inputMode;
};

