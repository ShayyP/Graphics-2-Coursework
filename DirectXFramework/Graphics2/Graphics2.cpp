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

XMFLOAT4X4* Graphics2::RotateAround(XMFLOAT4X4* WorldTransform, float angle)
{
	return nullptr;
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph

	_rotation += 1.0f;

	sceneGraph->SetWorldTransform(XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), _rotation * 0.5f * XM_PI / 180.0f));

	SceneNodePointer leftArm = sceneGraph->Find(L"Left Arm");
	leftArm->SetWorldTransform(XMLoadFloat4x4(leftArm->GetWorldTransform()) * XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), 0.5f * XM_PI / 180.0f));
}
