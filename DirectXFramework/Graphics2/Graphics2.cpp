#include "Graphics2.h"
#include "MeshNode.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	_inputMode = Controller;
	SceneGraphPointer sceneGraph = GetSceneGraph();
	GetCamera()->SetCameraPosition(0.0f, 50.0f, -500.0f);

	/*
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

	shared_ptr<MeshNode> plane = make_shared<MeshNode>(L"Plane", L"Textures\\Plane\\Bonanza.3DS");
	sceneGraph->Add(plane);

	shared_ptr<TerrainNode> terrain = make_shared<TerrainNode>(L"Terrain", L"");
	sceneGraph->Add(terrain);
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph

	_rotation += 1.0f;

	//sceneGraph->SetWorldTransform(XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), _rotation * XM_PI / 180.0f));

	SceneNodePointer plane = sceneGraph->Find(L"Plane");
	plane->SetWorldTransform(XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), 90 * XM_PI / 180.0f) * XMMatrixRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), 45 * XM_PI / 180.0f) * XMMatrixTranslation(50, 0, 0) * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), _rotation * XM_PI / 180.0f));

	/*
	SceneNodePointer rightArm = sceneGraph->Find(L"Right Arm");
	SceneNodePointer leftArm = sceneGraph->Find(L"Left Arm");

	rightArm->SetWorldTransform((XMMatrixScaling(1, 8.5, 1) * XMMatrixTranslation(6, -8.5, 0) * XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), _rotation * 0.9f * XM_PI / 180.0f)) * XMMatrixTranslation(0, 30, 0));
	leftArm->SetWorldTransform((XMMatrixScaling(1, 8.5, 1) * XMMatrixTranslation(-6, -8.5, 0) * XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), _rotation * -0.9f * XM_PI / 180.0f)) * XMMatrixTranslation(0, 30, 0));
	*/

	// Handling input
	if (_inputMode == Keyboard)
	{
		HandleKeyboardInput();
	}
	else
	{
		HandleControllerInput();
	}
}

void Graphics2::HandleKeyboardInput()
{
	// Handling keyboard input
	// Movement
	// W key
	if (GetAsyncKeyState(0x57) < 0)
	{
		GetCamera()->SetForwardBack(_keyboardSpeedModifier);
	}
	// S key
	if (GetAsyncKeyState(0x53) < 0)
	{
		GetCamera()->SetForwardBack(-_keyboardSpeedModifier);
	}
	// D key
	if (GetAsyncKeyState(0x44) < 0)
	{
		GetCamera()->SetLeftRight(_keyboardSpeedModifier);
	}
	// A key
	if (GetAsyncKeyState(0x41) < 0)
	{
		GetCamera()->SetLeftRight(-_keyboardSpeedModifier);
	}
	// Rotation
	// Right arrow key
	if (GetAsyncKeyState(VK_RIGHT) < 0)
	{
		GetCamera()->SetYaw(_keyboardSpeedModifier);
	}
	// Left arrow key
	if (GetAsyncKeyState(VK_LEFT) < 0)
	{
		GetCamera()->SetYaw(-_keyboardSpeedModifier);
	}
	// Up arrow key
	if (GetAsyncKeyState(VK_UP) < 0)
	{
		GetCamera()->SetPitch(-_keyboardSpeedModifier);
	}
	// Down arrow key
	if (GetAsyncKeyState(VK_DOWN) < 0)
	{
		GetCamera()->SetPitch(_keyboardSpeedModifier);
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
