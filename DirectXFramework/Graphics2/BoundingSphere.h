#pragma once
#include "BoundingVolume.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "WireframeRenderer.h"

using namespace std;

// Class for the bounding sphere inheriting from bounding volume
// Handles all collision detection for nodes using sphere bounds
class BoundingSphere : public BoundingVolume
{
public:
	BoundingSphere(shared_ptr<Mesh> mesh);
	BoundingSphere(shared_ptr<SubMesh> subMesh);
	static float Distance(XMFLOAT3 point1, XMFLOAT3 point2);
	void BuildSphere(float radius, XMFLOAT3 centrePos);
	void MergeSubBounds();

	virtual void Update(FXMMATRIX& worldTransformation) { XMStoreFloat4x4(&_combinedWorldTransformation, worldTransformation); }
	virtual void Render();

	float GetRadius() { return _radius; }
	XMFLOAT3 GetCentrePos() { return _centrePos; }
	virtual vector<shared_ptr<BoundingSphere>> GetSubBounds() { return _subBounds; }

	virtual bool IsIntersecting(shared_ptr<BoundingVolume> otherVolume);
	virtual float IsIntersectingRay(XMVECTOR origin, XMVECTOR direction);

private:
	vector<shared_ptr<BoundingSphere>> _subBounds;
	float							   _radius;
	XMFLOAT3						   _centrePos;
	shared_ptr<WireframeRenderer>      _renderer;
};

