#pragma once

#include "Cube.h"
#include "TerrainNode.h"
#include "MoveableNode.h"
#include "SkyNode.h"
#include <algorithm>

// Main class that handles nodes and thier interactions
class Graphics2 : public DirectXFramework
{
public:
	enum class InputMode{ Keyboard, Controller };
	void CreateSceneGraph();
	void UpdateSceneGraph();
	void CheckForCollisions();
	void HandleKeyboardInput(shared_ptr<Camera> nodeToMove);
	void HandleControllerInput(shared_ptr<Camera> nodeToMove);
	void HandleClick(float mouseX, float mouseY);
	void OnFreeCam();
	void ChangeMoveSpeed(float amount);

private:
	float						  _rotation = 0;
	wchar_t*					  _brickTexture = L"Textures/Bricks.png";
	wchar_t*					  _woodTexture = L"Textures/Wood.png";
	wchar_t*					  _concreteTexture = L"Textures/Concrete.png";
	wchar_t*					  _woodblockTexture = L"Textures/woodblock.bmp";
	float						  _keyboardSpeedModifier = 3.0f;
	float						  _cameraSpeedLimiter = 10000.0f;
	float						  _minSpeed = 0.5f;
	float						  _movementSpeed = 1.0f;
	float						  _maxSpeed = 5.0f;
	InputMode					  _inputMode = InputMode::Keyboard;
	shared_ptr<MoveableNode>	  _controlledNode;
	bool						  _freeCam = false;
	bool						  _freeCamPressed = false;
	bool						  _rayPressed = false;
	bool						  _renderPressed = false;
	bool						  _switchInputPressed = false;
	vector<SceneNodePointer>	  _collidableNodes;
	vector<SceneNodePointer>	  _pickableNodes;
	shared_ptr<TerrainNode>		  _terrain;
	bool						  _crashed = false;
	bool						  _firstUpdate = true;
};

