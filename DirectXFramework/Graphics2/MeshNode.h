#pragma once
#include "SceneNode.h"
#include "DirectXFramework.h"
#include "MeshRenderer.h"
#include "BoundingSphere.h"

// Scene node that uses a mesh loaded by ASSIMP
class MeshNode : public SceneNode
{
public:
	MeshNode(wstring name, wstring modelName) : SceneNode(name) { _modelName = modelName; }

	bool Initialise();
	void Render();
	void Shutdown();

private:
	shared_ptr<MeshRenderer>		_renderer;
	wstring							_modelName;
	shared_ptr<ResourceManager>		_resourceManager;
	shared_ptr<Mesh>				_mesh;
};

