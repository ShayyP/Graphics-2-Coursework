#include "Graphics2.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	SceneNodePointer head = make_shared<Cube>(L"Head", _brickTexture);
	head->SetWorldTransform(XMMatrixTranslation(0, 34, 0));
	head->Update(XMMatrixScaling(3, 3, 3));
	sceneGraph->Add(head);

	SceneNodePointer body = make_shared<Cube>(L"Body", _woodTexture);
	body->SetWorldTransform(XMMatrixTranslation(0, 23, 0));
	body->Update(XMMatrixScaling(5, 8, 2.5));
	sceneGraph->Add(body);

	SceneNodePointer leftArm = make_shared<Cube>(L"Left Arm", _brickTexture);
	leftArm->SetWorldTransform(XMMatrixTranslation(-6, 22, 0));
	leftArm->Update(XMMatrixScaling(1, 8.5, 1));
	sceneGraph->Add(leftArm);

	SceneNodePointer rightArm = make_shared<Cube>(L"Right Arm", _brickTexture);
	rightArm->SetWorldTransform(XMMatrixTranslation(6, 22, 0));
	rightArm->Update(XMMatrixScaling(1, 8.5, 1));
	sceneGraph->Add(rightArm);

	SceneNodePointer leftLeg = make_shared<Cube>(L"Left Leg", _concreteTexture);
	leftLeg->SetWorldTransform(XMMatrixTranslation(-4, 7.5, 0));
	leftLeg->Update(XMMatrixScaling(1, 7.5, 1));
	sceneGraph->Add(leftLeg);

	SceneNodePointer rightLeg = make_shared<Cube>(L"Right Leg", _concreteTexture);
	rightLeg->SetWorldTransform(XMMatrixTranslation(4, 7.5, 0));
	rightLeg->Update(XMMatrixScaling(1, 7.5, 1));
	sceneGraph->Add(rightLeg);
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph

	_rotation += 1.0f;
	sceneGraph->Update(XMMatrixRotationAxis(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), _rotation * 0.5f * XM_PI / 180.0f));
}
