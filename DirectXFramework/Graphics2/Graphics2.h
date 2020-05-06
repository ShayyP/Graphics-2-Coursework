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
	void HandleKeyboardInput();
	void HandleControllerInput();
	void HandleClick(float mouseX, float mouseY);

private:
	float					 _rotation = 0;
	wchar_t*				 _brickTexture = L"Textures/Bricks.png";
	wchar_t*				 _woodTexture = L"Textures/Wood.png";
	wchar_t*				 _concreteTexture = L"Textures/Concrete.png";
	wchar_t*				 _woodblockTexture = L"Textures/woodblock.bmp";
	float					 _keyboardSpeedModifier = 3.0f;
	float					 _cameraSpeedLimiter = 10000.0f;
	float				     _mouseSpeedLimiter = 10.0f;
	InputMode				 _inputMode;
	shared_ptr<MoveableNode> _controlledNode;
	bool					 _freeCam = false;
	bool					 _freeCamPressed = false;
	bool				     _mouseClicked = false;
	bool					 _rPressed = false;
	vector<SceneNodePointer> _collidableNodes;
	vector<SceneNodePointer> _pickableNodes;
	shared_ptr<TerrainNode>  _terrain;
	bool					 _crashed = false;
};

