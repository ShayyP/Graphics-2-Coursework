#include "Graphics2.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	GetCamera()->SetPosition(0.0f, 0.0f, 0.0f);

	// Terrain
	shared_ptr<TerrainNode> terrain = make_shared<TerrainNode>(L"Terrain", L"HeightMaps\\Example_HeightMap.RAW");
	sceneGraph->Add(terrain);
	_terrain = terrain;

	// Sky
	shared_ptr<SkyNode> sky = make_shared<SkyNode>(L"Sky", L"skymap.dds", 5000);
	sceneGraph->Add(sky);

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

	// Plane
	shared_ptr<MoveableNode> plane = make_shared<MoveableNode>(L"Plane", L"Models\\Plane\\Bonanza.3DS");
	plane->SetDefaultTransformation(XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), 90 * XM_PI / 180.0f) * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), 180 * XM_PI / 180.0f));
	sceneGraph->Add(plane);
	_controlledNode = plane;
	_controlledNode->SetPosition(0, 500, 0);

	// Jets
	shared_ptr<MeshNode> jet = make_shared<MeshNode>(L"Jet", L"Models\\Jet\\Attacker.blend");
	jet->SetDefaultTransformation(XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), 90 * XM_PI / 180.0f));
	sceneGraph->Add(jet);
	_collidableNodes.push_back(jet);
	_pickableNodes.push_back(jet);

	shared_ptr<MeshNode> jet2 = make_shared<MeshNode>(L"Jet2", L"Models\\Jet\\Attacker.blend");
	jet2->SetDefaultTransformation(XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), 90 * XM_PI / 180.0f));
	sceneGraph->Add(jet2);
	_collidableNodes.push_back(jet2);
	_pickableNodes.push_back(jet2);

	shared_ptr<MeshNode> jet3 = make_shared<MeshNode>(L"Jet3", L"Models\\Jet\\Attacker.blend");
	jet3->SetDefaultTransformation(XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), 90 * XM_PI / 180.0f));
	sceneGraph->Add(jet3);
	_collidableNodes.push_back(jet3);
	_pickableNodes.push_back(jet3);

	shared_ptr<MeshNode> jet4 = make_shared<MeshNode>(L"Jet4", L"Models\\Jet\\Attacker.blend");
	jet4->SetDefaultTransformation(XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), 90 * XM_PI / 180.0f));
	sceneGraph->Add(jet4);
	_collidableNodes.push_back(jet4);
	_pickableNodes.push_back(jet4);

	// Big boy boeings
	shared_ptr<MeshNode> boeing = make_shared<MeshNode>(L"Boeing", L"Models\\Boeing\\787.3ds");
	boeing->SetDefaultTransformation(XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), 90 * XM_PI / 180.0f));
	sceneGraph->Add(boeing);
	_collidableNodes.push_back(boeing);
	_pickableNodes.push_back(boeing);

	shared_ptr<MeshNode> boeing2 = make_shared<MeshNode>(L"Boeing2", L"Models\\Boeing\\787.3ds");
	boeing2->SetDefaultTransformation(XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), 90 * XM_PI / 180.0f));
	sceneGraph->Add(boeing2);
	_collidableNodes.push_back(boeing2);
	_pickableNodes.push_back(boeing2);

	shared_ptr<MeshNode> boeing3 = make_shared<MeshNode>(L"Boeing3", L"Models\\Boeing\\787.3ds");
	boeing3->SetDefaultTransformation(XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), 90 * XM_PI / 180.0f));
	sceneGraph->Add(boeing3);
	_collidableNodes.push_back(boeing3);
	_pickableNodes.push_back(boeing3);

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
		if (_inputMode == InputMode::Keyboard)
		{
			HandleKeyboardInput();
		}
		else
		{
			HandleControllerInput();
		}

		// This is where you make any changes to the local world transformations to nodes
		// in the scene graph

		SceneNodePointer jet = sceneGraph->Find(L"Jet");
		if (!jet->IsDead())
		{
			jet->SetWorldTransform(XMLoadFloat4x4(&jet->GetDefaultTransformation()) * XMMatrixRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), 45 * XM_PI / 180.0f) * XMMatrixTranslation(200, 0, 0) * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), _rotation * XM_PI / 180.0f) * XMMatrixTranslation(-500, 500, 0));
		}
		SceneNodePointer jet2 = sceneGraph->Find(L"Jet2");
		if (!jet2->IsDead())
		{
			jet2->SetWorldTransform(XMLoadFloat4x4(&jet2->GetDefaultTransformation()) * XMMatrixRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), 45 * XM_PI / 180.0f) * XMMatrixTranslation(200, 0, 0) * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), (_rotation + 90) * XM_PI / 180.0f) * XMMatrixTranslation(-500, 500, 0));
		}
		SceneNodePointer jet3 = sceneGraph->Find(L"Jet3");
		if (!jet3->IsDead())
		{
			jet3->SetWorldTransform(XMLoadFloat4x4(&jet3->GetDefaultTransformation()) * XMMatrixRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), 45 * XM_PI / 180.0f) * XMMatrixTranslation(200, 0, 0) * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), (_rotation + 180) * XM_PI / 180.0f) * XMMatrixTranslation(-500, 500, 0));
		}
		SceneNodePointer jet4 = sceneGraph->Find(L"Jet4");
		if (!jet4->IsDead())
		{
			jet4->SetWorldTransform(XMLoadFloat4x4(&jet4->GetDefaultTransformation()) * XMMatrixRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), 45 * XM_PI / 180.0f) * XMMatrixTranslation(200, 0, 0) * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), (_rotation + 270) * XM_PI / 180.0f) * XMMatrixTranslation(-500, 500, 0));
		}
		SceneNodePointer boeing = sceneGraph->Find(L"Boeing");
		if (!boeing->IsDead())
		{
			boeing->SetWorldTransform(XMLoadFloat4x4(&boeing->GetDefaultTransformation()) * XMMatrixTranslation(-1200, _terrain->GetHeightAtPoint(-1200, 0), 0));
		}
		SceneNodePointer boeing2 = sceneGraph->Find(L"Boeing2");
		if (!boeing2->IsDead())
		{
			boeing2->SetWorldTransform(XMLoadFloat4x4(&boeing2->GetDefaultTransformation()) * XMMatrixTranslation(-1200, _terrain->GetHeightAtPoint(-1200, 100), 100));
		}
		SceneNodePointer boeing3 = sceneGraph->Find(L"Boeing3");
		if (!boeing3->IsDead())
		{
			boeing3->SetWorldTransform(XMLoadFloat4x4(&boeing3->GetDefaultTransformation()) * XMMatrixTranslation(-1200, _terrain->GetHeightAtPoint(-1200, 200), 200));
		}

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
			if (_freeCam)
			{
				OnFreeCam();
			}
		}
		// Handling input
		if (_inputMode == InputMode::Keyboard)
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
			_controlledNode->SetLeftRight(_keyboardSpeedModifier);
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
			_controlledNode->SetLeftRight(-_keyboardSpeedModifier);
		}
		else
		{
			GetCamera()->SetLeftRight(-_keyboardSpeedModifier);
		}
	}
	// Rotation
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
			if (_freeCam)
			{
				OnFreeCam();
			}
			_freeCamPressed = false;
		}
	}

	// Click
	if (GetAsyncKeyState(VK_LBUTTON) < 0)
	{
		_rayPressed = true;
	}
	else
	{
		if (_rayPressed)
		{
			POINT p;
			if (GetCursorPos(&p))
			{
				ScreenToClient(GetDXFramework()->GetHWnd(), &p);
				HandleClick(static_cast<float>(p.x), static_cast<float>(p.y));
			}
			_rayPressed = false;
		}
	}

	// Toggle rendering of bounding volumes with R
	if (GetAsyncKeyState(0x52) < 0)
	{
		_renderPressed = true;
	}
	else
	{
		if (_renderPressed)
		{
			GetSceneGraph()->ToggleRenderBoundingVolumes();
			_renderPressed = false;
		}
	}

	if (GetAsyncKeyState(0x58) < 0)
	{
		_switchInputPressed = true;
	}
	else
	{
		if (_switchInputPressed)
		{
			_inputMode = InputMode::Controller;
			_switchInputPressed = false;
		}
	}
}

void Graphics2::HandleControllerInput()
{
	// Handling controller input
	char* buttonPressed = GetController()->ProcessGameController();
	if (buttonPressed == "A")
	{
		_freeCamPressed = true;
	}
	else
	{
		if (_freeCamPressed)
		{
			_freeCam = !_freeCam;
			if (_freeCam)
			{
				OnFreeCam();
			}
			_freeCamPressed = false;
		}
	}

	if (buttonPressed == "RT")
	{
		_rayPressed = true;
	}
	else
	{
		if (_rayPressed)
		{
			HandleClick(static_cast<float>(GetDXFramework()->GetWindowWidth()/2), static_cast<float>(GetDXFramework()->GetWindowHeight() / 2));
			_rayPressed = false;
		}
	}

	if (buttonPressed == "X")
	{
		_renderPressed = true;
	}
	else
	{
		if (_renderPressed)
		{
			GetSceneGraph()->ToggleRenderBoundingVolumes();
			_renderPressed = false;
		}
	}

	if (buttonPressed == "Y")
	{
		_switchInputPressed = true;
	}
	else
	{
		if (_switchInputPressed)
		{
			_inputMode = InputMode::Keyboard;
			_switchInputPressed = false;
		}
	}

	if (_freeCam)
	{
		// Movement
		GetCamera()->SetForwardBack(GetController()->GetThumbLY() / _cameraSpeedLimiter);
		GetCamera()->SetLeftRight(GetController()->GetThumbLX() / _cameraSpeedLimiter);
		// Rotation
		GetCamera()->SetYaw(GetController()->GetThumbRX() / _cameraSpeedLimiter);
		GetCamera()->SetPitch(-GetController()->GetThumbRY() / _cameraSpeedLimiter);
	}
	else
	{
		// Movement
		_controlledNode->SetForwardBack(GetController()->GetThumbLY() / _cameraSpeedLimiter);
		_controlledNode->SetLeftRight(GetController()->GetThumbLX() / _cameraSpeedLimiter);
		// Rotation
		_controlledNode->SetYaw(GetController()->GetThumbRX() / _cameraSpeedLimiter);
		_controlledNode->SetPitch(-GetController()->GetThumbRY() / _cameraSpeedLimiter);
	}
}

void Graphics2::HandleClick(float mouseX, float mouseY)
{
	XMFLOAT4X4 projectionMatrix;
	XMStoreFloat4x4(&projectionMatrix, GetProjectionTransformation());
	// Compute picking ray in view space.
	float vx = (+2.0f * mouseX / GetWindowWidth() - 1.0f) /
		projectionMatrix(0, 0);
	float vy = (-2.0f * mouseY / GetWindowHeight() + 1.0f) /
		projectionMatrix(1, 1);

	// Ray definition in view space.
	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDirection = XMVectorSet(vx, vy, 1.0f, 0.0f);

	// Getting inverse view matrix to transform to world space
	XMMATRIX viewTransformation = GetCamera()->GetViewMatrix();
	XMMATRIX inverseViewTransformation = XMMatrixInverse(&XMMatrixDeterminant(viewTransformation), viewTransformation);

	// Applying inverse view transform to ray
	rayOrigin = XMVector4Transform(rayOrigin, inverseViewTransformation);
	rayDirection = XMVector4Transform(rayDirection, inverseViewTransformation);

	SceneNodePointer closestIntersectingNode = nullptr;
	float distanceToClosestNode = 0.0f;

	for (SceneNodePointer node : _pickableNodes)
	{
		float isIntersecting = node->IntersectingRay(rayOrigin, rayDirection);
		if (isIntersecting != 0.0f)
		{
			if (closestIntersectingNode == nullptr)
			{
				closestIntersectingNode = node;
				distanceToClosestNode = isIntersecting;
			}
			else
			{
				if (isIntersecting > distanceToClosestNode)
				{
					closestIntersectingNode = node;
					distanceToClosestNode = isIntersecting;
				}
			}
		}
	}

	if (closestIntersectingNode != nullptr)
	{
		closestIntersectingNode->Kill();

		vector<SceneNodePointer>::iterator pos = find(_collidableNodes.begin(), _collidableNodes.end(), closestIntersectingNode);
		if (pos != _collidableNodes.end())
		{
			_collidableNodes.erase(pos);
		}

		pos = find(_pickableNodes.begin(), _pickableNodes.end(), closestIntersectingNode);
		if (pos != _pickableNodes.end())
		{
			_pickableNodes.erase(pos);
		}
	}
}

void Graphics2::OnFreeCam()
{
	GetCamera()->SetTotalPitch(_controlledNode->GetPitch());
	GetCamera()->SetTotalRoll(_controlledNode->GetRoll());
	GetCamera()->SetTotalYaw(_controlledNode->GetYaw());
}
