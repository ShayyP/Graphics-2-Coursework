#pragma once
#include "core.h"
#include "DirectXCore.h"
#include "BoundingVolume.h"

using namespace std;

// Abstract base class for all nodes of the scene graph.  
// This scene graph implements the Composite Design Pattern

class SceneNode;

typedef shared_ptr<SceneNode>	SceneNodePointer;

class SceneNode : public enable_shared_from_this<SceneNode>
{
public:
	SceneNode(wstring name) { _name = name; XMStoreFloat4x4(&_worldTransformation, XMMatrixIdentity()), XMStoreFloat4x4(&_defaultTransformation, XMMatrixIdentity()); };
	~SceneNode(void) {};

	// Core methods
	virtual bool Initialise() = 0;
	virtual void Update(FXMMATRIX& currentWorldTransformation) 
	{ 
		XMStoreFloat4x4(&_combinedWorldTransformation, XMLoadFloat4x4(&_worldTransformation) * currentWorldTransformation); 
		if (_boundingVolume != nullptr)
		{
			_boundingVolume->Update(XMLoadFloat4x4(&_combinedWorldTransformation));
		}
	}
	virtual void Render() = 0;
	virtual void Shutdown() = 0;

	void SetWorldTransform(FXMMATRIX& worldTransformation) { XMStoreFloat4x4(&_worldTransformation, worldTransformation); }

	XMFLOAT4X4 GetWorldTransform() { return _worldTransformation; }
	XMFLOAT4X4 GetDefaultTransformation() { return _defaultTransformation; }
	void SetDefaultTransformation(XMMATRIX defaultTransformation) { XMStoreFloat4x4(&_defaultTransformation, defaultTransformation); }
		
	// Although only required in the composite class, these are provided
	// in order to simplify the code base.
	virtual void Add(SceneNodePointer node) {}
	virtual void Remove(SceneNodePointer node) {};
	virtual	SceneNodePointer Find(wstring name) { return (_name == name) ? shared_from_this() : nullptr; }

	shared_ptr<BoundingVolume> GetBoundingVolume()  { return _boundingVolume; }
	virtual bool IsColliding(shared_ptr<SceneNode> otherNode)
	{
		if (_boundingVolume != nullptr && otherNode->GetBoundingVolume() != nullptr)
		{
			return _boundingVolume->IsIntersecting(otherNode->GetBoundingVolume());
		}
		return false;
	}
	virtual float IntersectingRay(XMVECTOR origin, XMVECTOR direction)
	{
		if (_boundingVolume != nullptr)
		{
			return _boundingVolume->IsIntersectingRay(origin, direction);
		}
		return 0.0f;
	}
	void Kill() { _dead = true; }
	bool IsDead() { return _dead; }

protected:
	XMFLOAT4X4	               _worldTransformation;
	XMFLOAT4X4			       _combinedWorldTransformation;
	wstring				       _name;
	shared_ptr<BoundingVolume> _boundingVolume = nullptr;
	XMFLOAT4X4                 _defaultTransformation;
	bool                       _dead = false;
};

