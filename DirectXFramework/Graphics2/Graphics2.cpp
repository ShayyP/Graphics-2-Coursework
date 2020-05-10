#include "Graphics2.h"

Graphics2 app;

// Initialises the scene and any nodes we want to add to it
void Graphics2::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	GetCamera()->SetPosition(0.0f, 0.0f, 0.0f);

	// Creating nodes and adding them to the scenegraph
	// Terrain
	shared_ptr<TerrainNode> terrain = make_shared<TerrainNode>(L"Terrain", L"HeightMaps\\Example_HeightMap.RAW");
	sceneGraph->Add(terrain);
	_terrain = terrain;

	// Sky
	shared_ptr<SkyNode> sky = make_shared<SkyNode>(L"Sky", L"Textures\\skymap.dds", 5000);
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
	// Sets the plane as the controlled node
	_controlledNode = plane;
	_controlledNode->SetPosition(0, 500, 0);

	// The below are added to the list of collidable and pickable nodes
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

// Main loop called every frame that updates the scene and any nodes that we want to transform
void Graphics2::UpdateSceneGraph()
{
	shared_ptr<Camera> nodeToMove;

	// Update the controlled node
	_controlledNode->Update();

	// If we are in freecam mode, update the camera normally, if not then pass in the details of the controlled node so it can follow
	if (!_freeCam)
	{
		GetCamera()->Update(_controlledNode->GetPosition(), _controlledNode->GetYaw(), _controlledNode->GetPitch(), _controlledNode->GetRoll(), 100, -25);
		nodeToMove = _controlledNode;
	}
	else
	{
		GetCamera()->Update();
		nodeToMove = GetCamera();
	}

	// Handling input
	if (_inputMode == InputMode::Keyboard)
	{
		HandleKeyboardInput(nodeToMove);
	}
	else
	{
		HandleControllerInput(nodeToMove);
	}

	// Update the scene graph if we have not crashed the plane, otherwise force freecam
	if (!_crashed)
	{
		// This is where you make any changes to the local world transformations to nodes
		// in the scene graph

		SceneGraphPointer sceneGraph = GetSceneGraph();

		if (_firstUpdate)
		{
			// Place boeings on the terrain, this only needs to be done once in the first update as they are not moving
			SceneNodePointer boeing = sceneGraph->Find(L"Boeing");
			SceneNodePointer boeing2 = sceneGraph->Find(L"Boeing2");
			SceneNodePointer boeing3 = sceneGraph->Find(L"Boeing3");

			boeing->SetWorldTransform(XMLoadFloat4x4(&boeing->GetDefaultTransformation()) * XMMatrixTranslation(-1200, _terrain->GetHeightAtPoint(-1200, 0), 0));
			boeing2->SetWorldTransform(XMLoadFloat4x4(&boeing2->GetDefaultTransformation()) * XMMatrixTranslation(-1200, _terrain->GetHeightAtPoint(-1200, 100), 100));
			boeing3->SetWorldTransform(XMLoadFloat4x4(&boeing3->GetDefaultTransformation()) * XMMatrixTranslation(-1200, _terrain->GetHeightAtPoint(-1200, 200), 200));

			_firstUpdate = false;
		}

		// Make jets fly in a circle, if they are not dead
		_rotation += 1.0f;
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
		GetCamera()->Update();
	}
}

// Checks for collisions with terrain or other nodes
void Graphics2::CheckForCollisions()
{
	// Checks for collision between the controlled node and the terrain
	if (_terrain->NodeHitFloor(_controlledNode))
	{
		_crashed = true;
	}

	// Checks for collision between the controlled node and all collidable nodes, 
	// they are only fully checked if they are close together (this is done in BoundingSphere.cpp)
	for (SceneNodePointer node : _collidableNodes)
	{
		if (_controlledNode->IsColliding(node))
		{
			_crashed = true;
		}
	}
}

// Applies keyboard and mouse input to the specified camera or controlled node
void Graphics2::HandleKeyboardInput(shared_ptr<Camera> nodeToMove)
{
	// Handling keyboard input
	// Movement
	// W key
	if (GetAsyncKeyState(0x57) < 0)
	{
		nodeToMove->SetForwardBack(_keyboardSpeedModifier * _movementSpeed);
	}
	// S key
	if (GetAsyncKeyState(0x53) < 0)
	{
		nodeToMove->SetForwardBack(-_keyboardSpeedModifier * _movementSpeed);
	}
	// D key
	if (GetAsyncKeyState(0x44) < 0)
	{
		nodeToMove->SetLeftRight(_keyboardSpeedModifier * _movementSpeed);
	}
	// A key
	if (GetAsyncKeyState(0x41) < 0)
	{
		nodeToMove->SetLeftRight(-_keyboardSpeedModifier * _movementSpeed);
	}
	// Changing move speed
	// F key
	if (GetAsyncKeyState(0x46) < 0)
	{
		ChangeMoveSpeed(0.05f);
	}
	// C key
	if (GetAsyncKeyState(0x43) < 0)
	{
		ChangeMoveSpeed(-0.05f);
	}
	// Rotation
	// E key
	if (GetAsyncKeyState(0x45) < 0)
	{
		nodeToMove->SetYaw(_keyboardSpeedModifier);
	}
	// Q key
	if (GetAsyncKeyState(0x51) < 0)
	{
		nodeToMove->SetYaw(-_keyboardSpeedModifier);
	}
	// Shift key
	if (GetAsyncKeyState(VK_SHIFT) < 0)
	{
		nodeToMove->SetPitch(-_keyboardSpeedModifier);
	}
	// CTRL key
	if (GetAsyncKeyState(VK_CONTROL) < 0)
	{
		nodeToMove->SetPitch(_keyboardSpeedModifier);
	}

	// The indented if/else statements below are to ensure that the action is only performed once when the key is pressed
	// Binding the functionality directly to the key press would cause it to be called every frame while the key is down which we do not want

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

	// Make ray with left mouse button click
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
				// Convert screen coordinates to client coordinated
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

	// Switch input method to controller with X
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

// Applies controller input to the specified camera or controlled node
void Graphics2::HandleControllerInput(shared_ptr<Camera> nodeToMove)
{
	// Handling controller input

	// Applies controller input and returns button that was pressed
	char* buttonPressed = GetController()->ProcessGameController();

	// Toggle freecam with A button
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

	// Make ray with right trigger
	if (buttonPressed == "RT")
	{
		_rayPressed = true;
	}
	else
	{
		if (_rayPressed)
		{
			// Ray target is centre of the screen
			HandleClick(static_cast<float>(GetDXFramework()->GetWindowWidth()/2), static_cast<float>(GetDXFramework()->GetWindowHeight() / 2));
			_rayPressed = false;
		}
	}

	// Toggle rendering of bounding volumes with X
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

	// Switch to keyboard and mouse input with Y
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

	// Change move speed with left and right bumper
	if (buttonPressed == "LB")
	{
		ChangeMoveSpeed(-0.05f);
	}
	if (buttonPressed == "RB")
	{
		ChangeMoveSpeed(0.05f);
	}

	// Movement
	nodeToMove->SetForwardBack((GetController()->GetThumbLY() / _cameraSpeedLimiter) * _movementSpeed);
	nodeToMove->SetLeftRight((GetController()->GetThumbLX() / _cameraSpeedLimiter) * _movementSpeed);
	// Rotation
	nodeToMove->SetYaw(GetController()->GetThumbRX() / _cameraSpeedLimiter);
	nodeToMove->SetPitch(-GetController()->GetThumbRY() / _cameraSpeedLimiter);
}

// Creates a ray and handles any intersections with pickable nodes
void Graphics2::HandleClick(float mouseX, float mouseY)
{
	// Converting the mouse coordinates to a position and direction in world space using vector magic
	XMFLOAT4X4 projectionMatrix;
	XMStoreFloat4x4(&projectionMatrix, GetProjectionTransformation());
	// Compute picking ray in view space.
	float vx = (+2.0f * mouseX / GetWindowWidth() - 1.0f) / projectionMatrix(0, 0);
	float vy = (-2.0f * mouseY / GetWindowHeight() + 1.0f) / projectionMatrix(1, 1);

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

	// Checks if the ray intersects any pickable nodes. Only saves the closest node that it intersects
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
				if (isIntersecting < distanceToClosestNode)
				{
					closestIntersectingNode = node;
					distanceToClosestNode = isIntersecting;
				}
			}
		}
	}

	// Destroys the closest node that the ray intersected, if it intersected any pickable nodes
	if (closestIntersectingNode != nullptr)
	{
		closestIntersectingNode->Kill();

		// Removes the node from the collidable nodes and pickable nodes vectors so that it cannot be collided with or picked from now on
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

// Fixes the camera to face the direction of the controlled node upon entering freecam mode
void Graphics2::OnFreeCam()
{
	GetCamera()->SetTotalPitch(_controlledNode->GetPitch());
	GetCamera()->SetTotalRoll(_controlledNode->GetRoll());
	GetCamera()->SetTotalYaw(_controlledNode->GetYaw());
}

// Applies acceleration to movement speed and clamps it
void Graphics2::ChangeMoveSpeed(float amount)
{
	_movementSpeed += amount;
	if (_movementSpeed < _minSpeed)
	{
		_movementSpeed = _minSpeed;
	}
	else if (_movementSpeed > _maxSpeed)
	{
		_movementSpeed = _maxSpeed;
	}
}
