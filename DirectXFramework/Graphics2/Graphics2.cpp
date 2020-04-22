#include "Graphics2.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	_inputMode = Keyboard;
	_turned = None;

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
	_plane = plane;
	_plane->SetPosition(0, 500, 0);

	shared_ptr<TerrainNode> terrain = make_shared<TerrainNode>(L"Terrain", L"HeightMaps\\Example_HeightMap.RAW");
	sceneGraph->Add(terrain);
	_terrain = terrain;

	shared_ptr<SkyNode> sky = make_shared<SkyNode>(L"Sky", L"skymap.dds", 5000);
	sceneGraph->Add(sky);
}

void Graphics2::UpdateSceneGraph()
{
	// Update the camera
	_plane->Update();
	if (!_freeCam)
	{
		GetCamera()->Update(_plane->GetPosition(), _plane->GetYaw(), _plane->GetPitch(), _plane->GetRoll(), 100, -25);
	}
	else
	{
		GetCamera()->Update();
	}
	SceneGraphPointer sceneGraph = GetSceneGraph();
	_rotation += 1.0f;
	_turned = None;

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



	// Getting camera pos and forward vector
	XMVECTOR cameraPos = GetCamera()->GetCameraPosition();
	XMFLOAT3 cameraPosFloat;
	XMStoreFloat3(&cameraPosFloat, cameraPos);

	XMVECTOR cameraForward = GetCamera()->GetForwardVector();
	XMFLOAT3 cameraForwardFloat;
	XMStoreFloat3(&cameraForwardFloat, cameraForward);

	/*
	float y = _terrain->GetHeightAtPoint(cameraPosFloat.x, cameraPosFloat.z);
	GetCamera()->SetCameraPosition(cameraPosFloat.x, y + 20, cameraPosFloat.z);
	*/

	/*
	// Calculating smooth plane roll
	if (_turned == Right && _roll < 45)
	{
		_roll++;
	}
	else if (_turned == Left && _roll > -45)
	{
		_roll--;
	}
	else if (_turned == None && _roll != 0)
	{
		if (_roll < 0)
		{
			_roll++;
		}
		else
		{
			_roll--;
		}
	}
	*/

	/*
	// Cube man wavy arms
	SceneNodePointer rightArm = sceneGraph->Find(L"Right Arm");
	SceneNodePointer leftArm = sceneGraph->Find(L"Left Arm");

	rightArm->SetWorldTransform((XMMatrixScaling(1, 8.5, 1) * XMMatrixTranslation(6, -8.5, 0) * XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), _rotation * 0.9f * XM_PI / 180.0f)) * XMMatrixTranslation(0, 30, 0));
	leftArm->SetWorldTransform((XMMatrixScaling(1, 8.5, 1) * XMMatrixTranslation(-6, -8.5, 0) * XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), _rotation * -0.9f * XM_PI / 180.0f)) * XMMatrixTranslation(0, 30, 0));
	*/
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
			_plane->SetForwardBack(_keyboardSpeedModifier);
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
			_plane->SetForwardBack(-_keyboardSpeedModifier);
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
			_plane->SetLeftRight(_keyboardSpeedModifier);
			_turned = Right;
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
			_plane->SetLeftRight(-_keyboardSpeedModifier);
			_turned = Left;
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
			_plane->SetYaw(_keyboardSpeedModifier);
			_turned = Right;
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
			_plane->SetYaw(-_keyboardSpeedModifier);
			_turned = Left;
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
			_plane->SetPitch(-_keyboardSpeedModifier);
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
			_plane->SetPitch(_keyboardSpeedModifier);
		}
		else
		{
			GetCamera()->SetPitch(_keyboardSpeedModifier);
		}
	}

	// Free cam
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
