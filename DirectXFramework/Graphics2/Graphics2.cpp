#include "Graphics2.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	_inputMode = Keyboard;

	SceneGraphPointer sceneGraph = GetSceneGraph();
	GetCamera()->SetCameraPosition(0.0f, 0.0f, 0.0f);

	/*
	// Cube man
	SceneNodePointer head = make_shared<Cube>(L"Head", _brickTexture);
	head->SetWorldTransform(XMMatrixScaling(3, 3, 3) * XMMatrixTranslation(0, 34, 0));
	sceneGraph->Add(head);
	
	SceneNodePointer body = make_shared<Cube>(L"Body", _woodTexture);
	body->SetWorldTransform(XMMatrixScaling(5, 8, 2.5) * XMMatrixTranslation(0, 23, 0));
	sceneGraph->Add(body);

	SceneNodePointer leftArm = make_shared<Cube>(L"Left Arm", _brickTexture);
	leftArm->SetWorldTransform(XMMatrixScaling(1, 8.5, 1) * XMMatrixTranslation(-6, 22, 0));
	sceneGraph->Add(leftArm);

	SceneNodePointer rightArm = make_shared<Cube>(L"Right Arm", _brickTexture);
	rightArm->SetWorldTransform(XMMatrixScaling(1, 8.5, 1) * XMMatrixTranslation(6, 22, 0));
	sceneGraph->Add(rightArm);

	SceneNodePointer leftLeg = make_shared<Cube>(L"Left Leg", _concreteTexture);
	leftLeg->SetWorldTransform(XMMatrixScaling(1, 7.5, 1) * XMMatrixTranslation(-4, 7.5, 0));
	sceneGraph->Add(leftLeg);

	SceneNodePointer rightLeg = make_shared<Cube>(L"Right Leg", _concreteTexture);
	rightLeg->SetWorldTransform(XMMatrixScaling(1, 7.5, 1) * XMMatrixTranslation(4, 7.5, 0));
	sceneGraph->Add(rightLeg);
	*/

	// Planes
	shared_ptr<MoveableNode> plane = make_shared<MoveableNode>(L"Plane", L"Textures\\Plane\\Bonanza.3DS");
	sceneGraph->Add(plane);
	_controlledNode = plane;
	_controlledNode->SetPosition(0, 500, 0);

	shared_ptr<MoveableNode> plane2 = make_shared<MoveableNode>(L"Plane2", L"Textures\\Plane\\Bonanza.3DS");
	sceneGraph->Add(plane2);
	_collidableNodes.push_back(plane2);

	shared_ptr<TerrainNode> terrain = make_shared<TerrainNode>(L"Terrain", L"HeightMaps\\Example_HeightMap.RAW");
	sceneGraph->Add(terrain);
	_terrain = terrain;

	shared_ptr<SkyNode> sky = make_shared<SkyNode>(L"Sky", L"skymap.dds", 5000);
	sceneGraph->Add(sky);
}

void Graphics2::UpdateSceneGraph()
{
	if (!_crashed)
	{
		// Update the camera
		_controlledNode->Update();
		if (!_freeCam)
		{
			GetCamera()->Update(_controlledNode->GetPosition(), _controlledNode->GetYaw(), _controlledNode->GetPitch(), _controlledNode->GetRoll(), 100, -25);
		}
		else
		{
			GetCamera()->Update();
		}
		SceneGraphPointer sceneGraph = GetSceneGraph();

		_rotation += 1.0f;

		// Handling input
		if (_inputMode == Keyboard)
		{
			HandleKeyboardInput();
		}
		else
		{
			HandleControllerInput();
		}

		// This is where you make any changes to the local world transformations to nodes
		// in the scene graph

		SceneNodePointer plane = sceneGraph->Find(L"Plane2");
		plane->SetWorldTransform(XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), 90 * XM_PI / 180.0f) * XMMatrixRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), 45 * XM_PI / 180.0f) * XMMatrixTranslation(100, 0, 0) * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), _rotation * XM_PI / 180.0f) * XMMatrixTranslation(-500, 500, 0));

		/*
		// Cube man wavy arms
		SceneNodePointer rightArm = sceneGraph->Find(L"Right Arm");
		SceneNodePointer leftArm = sceneGraph->Find(L"Left Arm");

		rightArm->SetWorldTransform((XMMatrixScaling(1, 8.5, 1) * XMMatrixTranslation(6, -8.5, 0) * XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), _rotation * 0.9f * XM_PI / 180.0f)) * XMMatrixTranslation(0, 30, 0));
		leftArm->SetWorldTransform((XMMatrixScaling(1, 8.5, 1) * XMMatrixTranslation(-6, -8.5, 0) * XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), _rotation * -0.9f * XM_PI / 180.0f)) * XMMatrixTranslation(0, 30, 0));
		*/
	}
	else
	{
		if (!_freeCam)
		{
			_freeCam = true;
		}
		// Handling input
		if (_inputMode == Keyboard)
		{
			HandleKeyboardInput();
		}
		else
		{
			HandleControllerInput();
		}
		GetCamera()->Update();
	}
}

void Graphics2::CheckForCollisions()
{
	if (_terrain->NodeHitFloor(_controlledNode))
	{
		_crashed = true;
	}

	for (SceneNodePointer node : _collidableNodes)
	{
		if (_controlledNode->IsColliding(node))
		{
			_crashed = true;
		}
	}
}

void Graphics2::HandleKeyboardInput()
{
	// Handling keyboard input
	// Movement
	// W key
	if (GetAsyncKeyState(0x57) < 0)
	{
		if (!_freeCam)
		{
			_controlledNode->SetForwardBack(_keyboardSpeedModifier);
		}
		else
		{
			GetCamera()->SetForwardBack(_keyboardSpeedModifier);
		}
	}
	// S key
	if (GetAsyncKeyState(0x53) < 0)
	{
		if (!_freeCam)
		{
			_controlledNode->SetForwardBack(-_keyboardSpeedModifier);
		}
		else
		{
			GetCamera()->SetForwardBack(-_keyboardSpeedModifier);
		}
	}
	// D key
	if (GetAsyncKeyState(0x44) < 0)
	{
		if (!_freeCam)
		{
			_controlledNode->SetYaw(_keyboardSpeedModifier);
		}
		else
		{
			GetCamera()->SetLeftRight(_keyboardSpeedModifier);
		}
	}
	// A key
	if (GetAsyncKeyState(0x41) < 0)
	{
		if (!_freeCam)
		{
			_controlledNode->SetYaw(-_keyboardSpeedModifier);
		}
		else
		{
			GetCamera()->SetLeftRight(-_keyboardSpeedModifier);
		}
	}
	// Rotation
	/*
	// Using mouse
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	POINT centerPos;
	centerPos.x = DirectXFramework::GetWindowWidth() / 2;
	centerPos.y = DirectXFramework::GetWindowHeight() / 2;
	if (ScreenToClient(DirectXFramework::GetHWnd(), &cursorPos))
	{
		GetCamera()->SetTotalYaw(-(centerPos.x - cursorPos.x) / _mouseSpeedLimiter);
		GetCamera()->SetTotalPitch(-(centerPos.y - cursorPos.y) / _mouseSpeedLimiter);
	}
	// Reset mouse pos
	if (GetAsyncKeyState(VK_RBUTTON) < 0)
	{
		ClientToScreen(DirectXFramework::GetHWnd(), &centerPos);
		SetCursorPos(centerPos.x, centerPos.y);
	}
	*/
	// Using keys
	// E key
	if (GetAsyncKeyState(0x45) < 0)
	{
		if (!_freeCam)
		{
			_controlledNode->SetYaw(_keyboardSpeedModifier);
		}
		else
		{
			GetCamera()->SetYaw(_keyboardSpeedModifier);
		}
	}
	// Q key
	if (GetAsyncKeyState(0x51) < 0)
	{
		if (!_freeCam)
		{
			_controlledNode->SetYaw(-_keyboardSpeedModifier);
		}
		else
		{
			GetCamera()->SetYaw(-_keyboardSpeedModifier);
		}
	}
	// Shift key
	if (GetAsyncKeyState(VK_SHIFT) < 0)
	{
		if (!_freeCam)
		{
			_controlledNode->SetPitch(-_keyboardSpeedModifier);
		}
		else
		{
			GetCamera()->SetPitch(-_keyboardSpeedModifier);
		}
	}
	// CTRL key
	if (GetAsyncKeyState(VK_CONTROL) < 0)
	{
		if (!_freeCam)
		{
			_controlledNode->SetPitch(_keyboardSpeedModifier);
		}
		else
		{
			GetCamera()->SetPitch(_keyboardSpeedModifier);
		}
	}

	// Free cam (space)
	if (GetAsyncKeyState(VK_SPACE) < 0)
	{
		_freeCamPressed = true;
	}
	else
	{
		if (_freeCamPressed)
		{
			_freeCam = !_freeCam;
			_freeCamPressed = false;
		}
	}
}

void Graphics2::HandleControllerInput()
{
	// Handling controller input
	GetController()->ProcessGameController();
	// Movement
	GetCamera()->SetForwardBack(GetController()->GetThumbLY() / _cameraSpeedLimiter);
	GetCamera()->SetLeftRight(GetController()->GetThumbLX() / _cameraSpeedLimiter);
	// Rotation
	GetCamera()->SetYaw(GetController()->GetThumbRX() / _cameraSpeedLimiter);
	GetCamera()->SetPitch(-GetController()->GetThumbRY() / _cameraSpeedLimiter);
}
