#include "Graphics2.h"

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

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
}

XMMATRIX Graphics2::RotateAround(XMFLOAT4X4* worldTransform, XMVECTOR axis, float angle)
{
	XMMATRIX offSet = XMMatrixTranslation(0, 0, 0);
	XMMATRIX rotation = XMMatrixRotationAxis(axis, angle * XM_PI / 180.f);
	XMMATRIX positioning = XMMatrixTranslation(worldTransform->_41, worldTransform->_42, worldTransform->_43);
	XMMATRIX scaling = XMMatrixScaling(worldTransform->_11, worldTransform->_22, worldTransform->_33);
	return offSet * rotation * positioning * scaling;
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph

	_rotation += 1.0f;

	sceneGraph->SetWorldTransform(XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), _rotation * XM_PI / 180.0f));

	SceneNodePointer leftArm = sceneGraph->Find(L"Left Arm");
	leftArm->SetWorldTransform(RotateAround(leftArm->GetWorldTransform(), XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), _rotation));
}
