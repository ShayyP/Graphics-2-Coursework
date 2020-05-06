#pragma once
#include "core.h"
#include "DirectXCore.h"
#include <vector>
#include "Structs.h"

using namespace std;

// Base class for bounding volumes, has virtual methods that are overridden by child classes
class BoundingVolume : public enable_shared_from_this<BoundingVolume>
{
public:
	BoundingVolume() { XMStoreFloat4x4(&_combinedWorldTransformation, XMMatrixIdentity());  }
	virtual bool IsIntersecting(shared_ptr<BoundingVolume> otherVolume) { return false; }
	virtual float IsIntersectingRay(XMVECTOR origin, XMVECTOR direction) { return false; }
	virtual void Update(FXMMATRIX& worldTransformation) {}
	virtual void Render() {};

	XMFLOAT4X4 GetCombinedWorldTransformation() { return _combinedWorldTransformation; }
	vector<WireframeVertex> GetVertices() { return _vertices; }
	vector<UINT> GetIndices() { return _indices; }
	float GetMaxSize() { return _maxSize; }

protected:
	XMFLOAT4X4			    _combinedWorldTransformation;
	vector<WireframeVertex> _vertices;
	vector<UINT>	        _indices;
	float				    _maxSize = 0.0f;
};

