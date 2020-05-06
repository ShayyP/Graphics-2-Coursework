#include "BoundingSphere.h"

// Constructor that takes a mesh
BoundingSphere::BoundingSphere(shared_ptr<Mesh> mesh) : BoundingVolume()
{
	// Makes a bounding sphere for each submesh and adds them to a vector
	for (shared_ptr<SubMesh> subMesh : mesh->GetSubMeshList())
	{
		_subBounds.push_back(make_shared<BoundingSphere>(subMesh));
	}

	// Saves the biggest radius for later use
	for (shared_ptr<BoundingVolume> subBounds : _subBounds)
	{
		if (subBounds->GetMaxSize() > _maxSize)
		{
			_maxSize = subBounds->GetMaxSize();
		}
	}

	// Merges all of the bounding spheres for the submeshes into one vertex and index buffer
	MergeSubBounds();

	// Creates the renderer and initialises it with the vertex and index buffers
	_renderer = make_shared<WireframeRenderer>();
	_renderer->Initialise(_vertices, _indices);
}

// Constructor that takes a submesh
BoundingSphere::BoundingSphere(shared_ptr<SubMesh> subMesh)
{
	// Ritter algorithm
	XMFLOAT3 minX = subMesh->GetPositions()[0];
	XMFLOAT3 maxX = minX;
	XMFLOAT3 minY = minX;
	XMFLOAT3 maxY = minX;
	XMFLOAT3 minZ = minX;
	XMFLOAT3 maxZ = minX;

	// Gets min and max vertex for each axes
	for (XMFLOAT3 vertex : subMesh->GetPositions())
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

	// Gets the pair with the largest distance between them
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

	// Calculates the centre pos and radius of the sphere
	XMFLOAT3 centrePos = XMFLOAT3(largestPairMin.x + ((largestPairMax.x - largestPairMin.x) / 2), largestPairMin.y + ((largestPairMax.y - largestPairMin.y) / 2), largestPairMin.z + ((largestPairMax.z - largestPairMin.z) / 2));
	float radius = largestDistance / 2;
	if (largestDistance > _maxSize)
	{
		_maxSize = largestDistance;
	}

	// If a vertex is outside of the sphere, move the sphere towards it and increase the radius
	for (XMFLOAT3 vertex : subMesh->GetPositions())
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

	// Build vertices and indices for the sphere
	BuildSphere(radius, centrePos);
}

// Returns the distance between two XMFLOAT3 points
float BoundingSphere::Distance(XMFLOAT3 point1, XMFLOAT3 point2)
{
	return sqrt(pow((point2.x - point1.x), 2) + pow((point2.y - point1.y), 2) + pow((point2.z - point1.z), 2));
}

void BoundingSphere::BuildSphere(float radius, XMFLOAT3 centrePos)
{
	// Saves radius and centre pos for later
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

			// Adds the vertex to the vertices vector
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
		// Adds all vertices from all sub meshes to a single vector
		size_t verticesLength = _vertices.size();
		for (WireframeVertex vertex : subBound->GetVertices())
		{
			_vertices.push_back(vertex);
		}
		// Does the same for indices but adds the length of the vertex vector to ensure that the indices are pointing at the correct vertices
		// For example if index is 5 and there are already 54 vertices from other submeshes, the vertex at position 5 in the submesh will be 
		// at position 54+5 in the merged vector so the new index is 59
		for (UINT index : subBound->GetIndices())
		{
			_indices.push_back(index + static_cast<UINT>(verticesLength));
		}
	}
}

void BoundingSphere::Render()
{
	// Calls the render method in the wireframe renderer instance, passing the combined world transformation
	_renderer->Render(XMLoadFloat4x4(&_combinedWorldTransformation));
}

// Returns true if the bound is colliding with the other bound
bool BoundingSphere::IsIntersecting(shared_ptr<BoundingVolume> otherVolume)
{
	// Only do a full check if the objects are close, otherwise we are just wasting resources as the full check is somewhat costly with lots of nested iteration
	float otherMaxSize = otherVolume->GetMaxSize();
	XMFLOAT4X4 otherPos = otherVolume->GetCombinedWorldTransformation();
	float distanceBetweenVolumes = Distance(XMFLOAT3(_combinedWorldTransformation._41, _combinedWorldTransformation._42, _combinedWorldTransformation._43), XMFLOAT3(otherPos._41, otherPos._42, otherPos._43));
	if (distanceBetweenVolumes < (_maxSize + otherMaxSize))
	{
		// Assumes other volume is a sphere, which will need to change if different bound shapes are added
		// This cast is needed to access methods that are only available in the bounding sphere class
		shared_ptr<BoundingSphere> otherBound = dynamic_pointer_cast<BoundingSphere>(otherVolume);

		// Loops through all sub bounds
		for (shared_ptr<BoundingSphere> subBound : _subBounds)
		{
			float radius = subBound->GetRadius();
			XMFLOAT3 centrePos = subBound->GetCentrePos();
			XMFLOAT4X4 centreWorldPos;
			// Applies combined world tranformation to the centre pos
			XMStoreFloat4x4(&centreWorldPos, XMMatrixTranslation(centrePos.x, centrePos.y, centrePos.z) * XMLoadFloat4x4(&_combinedWorldTransformation));
			centrePos = XMFLOAT3(centreWorldPos._41, centreWorldPos._42, centreWorldPos._43);

			// Loops through all sub bounds in the other volume, meaning that all sub bounds are compared
			for (shared_ptr<BoundingSphere> otherSubBound : otherBound->GetSubBounds())
			{
				float otherRadius = subBound->GetRadius();
				XMFLOAT3 otherCentrePos = subBound->GetCentrePos();
				XMFLOAT4X4 otherCentreWorldPos;
				// Applies combined world tranformation to the centre pos
				XMStoreFloat4x4(&otherCentreWorldPos, XMMatrixTranslation(otherCentrePos.x, otherCentrePos.y, otherCentrePos.z) * XMLoadFloat4x4(&otherBound->GetCombinedWorldTransformation()));
				otherCentrePos = XMFLOAT3(otherCentreWorldPos._41, otherCentreWorldPos._42, otherCentreWorldPos._43);
			
				// If the distance between the two centre points is less than or equal to the sum of the two radiuses, then the spheres are overlapping and the objects are colliding
				if (Distance(centrePos, otherCentrePos) <= (radius + otherRadius))
				{
					return true;
				}
			}
		}
	}
	return false;
}

// Returns the distance of the intersection point from the camera if the ray intersects the sphere, if not it returns 0
float BoundingSphere::IsIntersectingRay(XMVECTOR origin, XMVECTOR direction)
{
	// Loops throuh all sub bounds
	for (shared_ptr<BoundingSphere> subBound : _subBounds)
	{
		float radius = subBound->GetRadius();
		XMFLOAT3 centrePos = subBound->GetCentrePos();
		XMFLOAT4X4 centreWorldPos;
		// Applies world transform to centre pos
		XMStoreFloat4x4(&centreWorldPos, XMMatrixTranslation(centrePos.x, centrePos.y, centrePos.z) * XMLoadFloat4x4(&_combinedWorldTransformation));
		centrePos = XMFLOAT3(centreWorldPos._41, centreWorldPos._42, centreWorldPos._43);
		XMVECTOR centrePosVector = XMLoadFloat3(&XMFLOAT3(centreWorldPos._41, centreWorldPos._42, centreWorldPos._43));

		// Normalizes the direction vector
		direction = XMVector3Normalize(direction);
		// v = vector from the centre pos to the origin of the ray
		XMVECTOR v = centrePosVector - origin;
		// Calculates the dot product of v and the direction of the ray, 
		// this is the distance from the origin of the ray to the point on the line that is closest to the centre pos
		// *Cool vector magic*
		XMFLOAT3 dot;
		XMStoreFloat3(&dot, XMVector3Dot(v, direction));
		// Calculates the point on the line that is the closest to the centre pos by adding the distance * direction to the origin point of the ray
		XMFLOAT3 closestPointOnLine;
		XMStoreFloat3(&closestPointOnLine, origin + (direction * dot.x));

		// If the distance between the closest point of the line and the centre pos is less than or equal to the radius of the sphere,
		// then the ray is intersecting the sphere
		if (Distance(closestPointOnLine, centrePos) <= radius)
		{
			XMFLOAT3 distanceOfNodeFromCamera;
			XMStoreFloat3(&distanceOfNodeFromCamera, XMVector3Length(v));
			return distanceOfNodeFromCamera.x;
		}
	}
	return 0.0f;
}
