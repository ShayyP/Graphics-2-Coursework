#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(shared_ptr<Mesh> mesh) : BoundingVolume()
{
	_mesh = mesh;

	for (shared_ptr<SubMesh> subMesh : _mesh->GetSubMeshList())
	{
		_subBounds.push_back(make_shared<BoundingSphere>(subMesh));
	}

	for (shared_ptr<BoundingVolume> subBounds : _subBounds)
	{
		if (subBounds->GetMaxSize() > _maxSize)
		{
			_maxSize = subBounds->GetMaxSize();
		}
	}

	MergeSubBounds();
	_renderer = make_shared<WireframeRenderer>();
	_renderer->Initialise(_vertices, _indices);
}

BoundingSphere::BoundingSphere(shared_ptr<SubMesh> subMesh)
{
	_subMesh = subMesh;
	
	// Ritter algorithm

	XMFLOAT3 minX = _subMesh->GetPositions()[0];
	XMFLOAT3 maxX = minX;
	XMFLOAT3 minY = minX;
	XMFLOAT3 maxY = minX;
	XMFLOAT3 minZ = minX;
	XMFLOAT3 maxZ = minX;

	for (XMFLOAT3 vertex : _subMesh->GetPositions())
	{
		if (vertex.x < minX.x) { minX = vertex; }
		if (vertex.x > maxX.x) { maxX = vertex; }
		if (vertex.y < minY.y) { minY = vertex; }
		if (vertex.y > maxY.y) { maxY = vertex; }
		if (vertex.z < minZ.z) { minZ = vertex; }
		if (vertex.z > maxZ.z) { maxZ = vertex; }
	}

	float largestDistance = 0.0f;
	XMFLOAT3 largestPairMin = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 largestPairMax = XMFLOAT3(0.0f, 0.0f, 0.0f);

	float xDistance = Distance(minX, maxX);
	float yDistance = Distance(minY, maxY);
	float zDistance = Distance(minZ, maxZ);

	if (xDistance > largestDistance)
	{
		largestDistance = xDistance;
		largestPairMin = minX;
		largestPairMax = maxX;
	}
	if (yDistance > largestDistance)
	{
		largestDistance = yDistance;
		largestPairMin = minY;
		largestPairMax = maxY;
	}
	if (zDistance > largestDistance)
	{
		largestDistance = zDistance;
		largestPairMin = minZ;
		largestPairMax = maxZ;
	}

	XMFLOAT3 centrePos = XMFLOAT3(largestPairMin.x + ((largestPairMax.x - largestPairMin.x) / 2), largestPairMin.y + ((largestPairMax.y - largestPairMin.y) / 2), largestPairMin.z + ((largestPairMax.z - largestPairMin.z) / 2));
	float radius = largestDistance / 2;
	if (largestDistance > _maxSize)
	{
		_maxSize = largestDistance;
	}

	for (XMFLOAT3 vertex : _subMesh->GetPositions())
	{
		float distance = Distance(vertex, centrePos);
		if (distance > radius)
		{
			float moveAmount = (distance - radius) / 2;
			if (vertex.x < centrePos.x) { centrePos.x -= moveAmount; }
			else { centrePos.x += moveAmount; }
			if (vertex.y < centrePos.y) { centrePos.y -= moveAmount; }
			else { centrePos.y += moveAmount; }
			if (vertex.z < centrePos.z) { centrePos.z -= moveAmount; }
			else { centrePos.z += moveAmount; }

			radius = (distance + radius) / 2;
		}
	}

	BuildSphere(radius, centrePos);
}

float BoundingSphere::Distance(XMFLOAT3 point1, XMFLOAT3 point2)
{
	return sqrt(pow((point2.x - point1.x), 2) + pow((point2.y - point1.y), 2) + pow((point2.z - point1.z), 2));
}

void BoundingSphere::BuildSphere(float radius, XMFLOAT3 centrePos)
{
	_radius = radius;
	_centrePos = centrePos;

	_vertices.clear();
	_indices.clear();

	size_t tesselation = 30;
	size_t verticalSegments = tesselation;
	size_t horizontalSegments = tesselation * 2;

	// Create rings of vertices at progressively higher latitudes.
	for (size_t i = 0; i <= verticalSegments; i++)
	{
		float v = 1 - (float)i / verticalSegments;

		float latitude = (i * XM_PI / verticalSegments) - XM_PIDIV2;
		float dy, dxz;

		XMScalarSinCos(&dy, &dxz, latitude);

		// Create a single ring of vertices at this latitude.
		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			float u = (float)j / horizontalSegments;

			float longitude = j * XM_2PI / horizontalSegments;
			float dx, dz;

			XMScalarSinCos(&dx, &dz, longitude);

			dx *= dxz;
			dz *= dxz;

			WireframeVertex vertex;
			vertex.Position.x = (dx * radius) + centrePos.x;
			vertex.Position.y = (dy * radius) + centrePos.y;
			vertex.Position.z = (dz * radius) + centrePos.z;
			vertex.Colour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			_vertices.push_back(vertex);
		}
	}

	// Fill the index buffer with triangles joining each pair of latitude rings.
	size_t stride = horizontalSegments + 1;

	for (size_t i = 0; i < verticalSegments; i++)
	{
		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			size_t nextI = i + 1;
			size_t nextJ = (j + 1) % stride;

			_indices.push_back((UINT)(i * stride + j));
			_indices.push_back((UINT)(nextI * stride + j));
			_indices.push_back((UINT)(i * stride + nextJ));

			_indices.push_back((UINT)(i * stride + nextJ));
			_indices.push_back((UINT)(nextI * stride + j));
			_indices.push_back((UINT)(nextI * stride + nextJ));
		}
	}
}

void BoundingSphere::MergeSubBounds()
{
	for (shared_ptr<BoundingSphere> subBound : _subBounds)
	{
		size_t verticesLength = _vertices.size();
		for (WireframeVertex vertex : subBound->GetVertices())
		{
			_vertices.push_back(vertex);
		}
		for (UINT index : subBound->GetIndices())
		{
			_indices.push_back(index + static_cast<UINT>(verticesLength));
		}
	}
}

void BoundingSphere::Render()
{
	_renderer->Render(XMLoadFloat4x4(&_combinedWorldTransformation));
}

bool BoundingSphere::IsIntersecting(shared_ptr<BoundingVolume> otherVolume)
{
	// Only do a full check if the objects are close, otherwise we are just wasting resources as the full check is somewhat costly
	float otherMaxSize = otherVolume->GetMaxSize();
	XMFLOAT4X4 otherPos = otherVolume->GetCombinedWorldTransformation();
	float distanceBetweenVolumes = Distance(XMFLOAT3(_combinedWorldTransformation._41, _combinedWorldTransformation._42, _combinedWorldTransformation._43), XMFLOAT3(otherPos._41, otherPos._42, otherPos._43));
	if (distanceBetweenVolumes < (_maxSize + otherMaxSize))
	{
		shared_ptr<BoundingSphere> otherBound = dynamic_pointer_cast<BoundingSphere>(otherVolume);

		for (shared_ptr<BoundingSphere> subBound : _subBounds)
		{
			float radius = subBound->GetRadius();
			XMFLOAT3 centrePos = subBound->GetCentrePos();
			XMFLOAT4X4 centreWorldPos;
			XMStoreFloat4x4(&centreWorldPos, XMMatrixTranslation(centrePos.x, centrePos.y, centrePos.z) * XMLoadFloat4x4(&_combinedWorldTransformation));
			centrePos = XMFLOAT3(centreWorldPos._41, centreWorldPos._42, centreWorldPos._43);

			for (shared_ptr<BoundingSphere> otherSubBound : otherBound->GetSubBounds())
			{
				float otherRadius = subBound->GetRadius();
				XMFLOAT3 otherCentrePos = subBound->GetCentrePos();
				XMFLOAT4X4 otherCentreWorldPos;
				XMStoreFloat4x4(&otherCentreWorldPos, XMMatrixTranslation(otherCentrePos.x, otherCentrePos.y, otherCentrePos.z) * XMLoadFloat4x4(&otherBound->GetCombinedWorldTransformation()));
				otherCentrePos = XMFLOAT3(otherCentreWorldPos._41, otherCentreWorldPos._42, otherCentreWorldPos._43);
			
				if (Distance(centrePos, otherCentrePos) <= (radius + otherRadius))
				{
					return true;
				}
			}
		}
	}
	return false;
}

float BoundingSphere::IsIntersectingRay(XMVECTOR origin, XMVECTOR direction)
{
	for (shared_ptr<BoundingSphere> subBound : _subBounds)
	{
		float radius = subBound->GetRadius();
		XMFLOAT3 centrePos = subBound->GetCentrePos();
		XMFLOAT4X4 centreWorldPos;
		XMStoreFloat4x4(&centreWorldPos, XMMatrixTranslation(centrePos.x, centrePos.y, centrePos.z) * XMLoadFloat4x4(&_combinedWorldTransformation));
		centrePos = XMFLOAT3(centreWorldPos._41, centreWorldPos._42, centreWorldPos._43);
		XMVECTOR centrePosVector = XMLoadFloat3(&XMFLOAT3(centreWorldPos._41, centreWorldPos._42, centreWorldPos._43));

		direction = XMVector3Normalize(direction);
		XMVECTOR v = centrePosVector - origin;
		XMFLOAT3 dot;
		XMStoreFloat3(&dot, XMVector3Dot(v, direction));
		XMFLOAT3 closestPointOnLine;
		XMStoreFloat3(&closestPointOnLine, origin + (direction * dot.x));

		if (Distance(closestPointOnLine, centrePos) <= radius * 5)
		{
			XMFLOAT3 distanceOfNodeFromCamera;
			XMStoreFloat3(&distanceOfNodeFromCamera, XMVector3Length(v));
			return distanceOfNodeFromCamera.x;
		}
	}
	return 0.0f;
}
