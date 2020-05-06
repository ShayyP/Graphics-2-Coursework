#pragma once
#include "MeshNode.h"
#include "Camera.h"

// Mesh node that can be moved by the user and the camera follows, inherits from camera as they share functionality
class MoveableNode : public MeshNode, public Camera
{
public:
	MoveableNode(wstring name, wstring modelName) : MeshNode(name, modelName), Camera() {}

	virtual void Update();
};

