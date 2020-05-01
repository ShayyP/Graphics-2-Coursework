#pragma once
#include "core.h"
#include "DirectXCore.h"
#include <vector>
#include "Structs.h"

//#define RENDER_BOUNDING_VOLUMES true;

using namespace std;

class BoundingVolume : public enable_shared_from_this<BoundingVolume>
{
public:
	BoundingVolume() { XMStoreFloat4x4(&_combinedWorldTransformation, XMMatrixIdentity());  }
	virtual bool IsIntersecting(shared_ptr<BoundingVolume> otherVolume) { return false; }
	virtual float IsIntersectingRay(XMVECTOR origin, XMVECTOR direction) { return false; }
	virtual void Update(FXMMATRIX& worldTransformation) {}

	XMFLOAT4X4 GetCombinedWorldTransformation() { return _combinedWorldTransformation; }
	vector<WireframeVertex> GetVertices() { return _vertices; }
	vector<UINT> GetIndices() { return _indices; }
	float GetMaxSize() { return _maxSize; }

#if defined RENDER_BOUNDING_VOLUMES
	virtual void Render() {};
#endif

protected:
	XMFLOAT4X4 _combinedWorldTransformation;
	vector<WireframeVertex> _vertices;
	vector<UINT> _indices;
	float _maxSize = 0.0f;
};

