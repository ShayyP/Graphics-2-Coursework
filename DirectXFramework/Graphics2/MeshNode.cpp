#include "MeshNode.h"

bool MeshNode::Initialise()
{
	_resourceManager = DirectXFramework::GetDXFramework()->GetResourceManager();
	_renderer = dynamic_pointer_cast<MeshRenderer>(_resourceManager->GetRenderer(L"PNT"));
	_mesh = _resourceManager->GetMesh(_modelName);
	if (_mesh == nullptr)
	{
		return false;
	}
	_boundingVolume = make_shared<BoundingSphere>(_mesh);
	return _renderer->Initialise();
}

void MeshNode::Shutdown()
{
	_resourceManager->ReleaseMesh(_modelName);
}

void MeshNode::Render()
{
	if (!_dead)
	{
		_renderer->SetMesh(_mesh);
		_renderer->SetWorldTransformation(XMLoadFloat4x4(&_combinedWorldTransformation));
		_renderer->SetCameraPosition(XMFLOAT4(0.0f, 0.0f, -100.0f, 1.0f));
		_renderer->SetAmbientLight(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
		_renderer->SetDirectionalLight(XMVectorSet(0.0f, -1.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		_renderer->Render();
		if (DirectXFramework::GetDXFramework()->GetSceneGraph()->GetRenderBoundingVolumes())
		{
			_boundingVolume->Render();
		}
	}
}

