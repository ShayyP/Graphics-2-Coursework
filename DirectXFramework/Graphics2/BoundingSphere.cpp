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
		subBounds->Initialise();
		if (subBounds->GetMaxSize() > _maxSize)
		{
			_maxSize = subBounds->GetMaxSize();
		}
	}

	MergeSubBounds();
	Initialise();
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

			BoundVertex vertex;
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
		for (BoundVertex vertex : subBound->GetVertices())
		{
			_vertices.push_back(vertex);
		}
		for (UINT index : subBound->GetIndices())
		{
			_indices.push_back(index + static_cast<UINT>(verticesLength));
		}
	}
}

void BoundingSphere::Initialise()
{
	BuildGeometryBuffers();
	BuildShaders();
	BuildVertexLayout();
	BuildConstantBuffer();
	BuildRendererStates();
}

void BoundingSphere::Render()
{
	// Calculate the world x view x projection transformation
	XMMATRIX completeTransformation = XMLoadFloat4x4(&_combinedWorldTransformation) * DirectXFramework::GetDXFramework()->GetCamera()->GetViewMatrix() * DirectXFramework::GetDXFramework()->GetProjectionTransformation();

	BoundCBUFFER cBuffer;
	cBuffer.CompleteTransformation = completeTransformation;

	_deviceContext->VSSetShader(_vertexShader.Get(), 0, 0);
	_deviceContext->PSSetShader(_pixelShader.Get(), 0, 0);
	_deviceContext->IASetInputLayout(_layout.Get());

	// Now render the sphere
	UINT stride = sizeof(BoundVertex);
	UINT offset = 0;
	_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Update the constant buffer 
	_deviceContext->UpdateSubresource(_constantBuffer.Get(), 0, 0, &cBuffer, 0, 0);
	_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	_deviceContext->PSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_deviceContext->RSSetState(_defaultRasteriserState.Get());
	_deviceContext->DrawIndexed(static_cast<UINT>(_indices.size()), 0, 0);
}

void BoundingSphere::BuildGeometryBuffers()
{
	// Setup the structure that specifies how big the vertex 
	// buffer should be
	D3D11_BUFFER_DESC vertexBufferDescriptor;
	vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescriptor.ByteWidth = sizeof(BoundVertex) * static_cast<UINT>(_vertices.size());
	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescriptor.CPUAccessFlags = 0;
	vertexBufferDescriptor.MiscFlags = 0;
	vertexBufferDescriptor.StructureByteStride = 0;

	// Now set up a structure that tells DirectX where to get the
	// data for the vertices from
	D3D11_SUBRESOURCE_DATA vertexInitialisationData;
	vertexInitialisationData.pSysMem = &_vertices[0];

	// and create the vertex buffer
	ThrowIfFailed(_device->CreateBuffer(&vertexBufferDescriptor, &vertexInitialisationData, _vertexBuffer.GetAddressOf()));

	// Setup the structure that specifies how big the index 
	// buffer should be
	D3D11_BUFFER_DESC indexBufferDescriptor;
	indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescriptor.ByteWidth = sizeof(UINT) * static_cast<UINT>(_indices.size());
	indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescriptor.CPUAccessFlags = 0;
	indexBufferDescriptor.MiscFlags = 0;
	indexBufferDescriptor.StructureByteStride = 0;

	// Now set up a structure that tells DirectX where to get the
	// data for the indices from
	D3D11_SUBRESOURCE_DATA indexInitialisationData;
	indexInitialisationData.pSysMem = &_indices[0];

	// and create the index buffer
	ThrowIfFailed(_device->CreateBuffer(&indexBufferDescriptor, &indexInitialisationData, _indexBuffer.GetAddressOf()));
}

void BoundingSphere::BuildShaders()
{
	DWORD shaderCompileFlags = 0;
#if defined( _DEBUG )
	shaderCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> compilationMessages = nullptr;

	//Compile vertex shader
	HRESULT hr = D3DCompileFromFile(L"WireframeShader.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		shaderCompileFlags, 0,
		_vertexShaderByteCode.GetAddressOf(),
		compilationMessages.GetAddressOf());

	if (compilationMessages.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
	}
	// Even if there are no compiler messages, check to make sure there were no other errors.
	ThrowIfFailed(hr);
	ThrowIfFailed(_device->CreateVertexShader(_vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), NULL, _vertexShader.GetAddressOf()));
	_deviceContext->VSSetShader(_vertexShader.Get(), 0, 0);

	// Compile pixel shader
	hr = D3DCompileFromFile(L"WireframeShader.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderCompileFlags, 0,
		_pixelShaderByteCode.GetAddressOf(),
		compilationMessages.GetAddressOf());

	if (compilationMessages.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
	}
	ThrowIfFailed(hr);
	ThrowIfFailed(_device->CreatePixelShader(_pixelShaderByteCode->GetBufferPointer(), _pixelShaderByteCode->GetBufferSize(), NULL, _pixelShader.GetAddressOf()));
	_deviceContext->PSSetShader(_pixelShader.Get(), 0, 0);
}

void BoundingSphere::BuildVertexLayout()
{
	// Create the vertex input layout. This tells DirectX the format
	// of each of the vertices we are sending to it.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOUR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ThrowIfFailed(_device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), _vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), _layout.GetAddressOf()));
	_deviceContext->IASetInputLayout(_layout.Get());
}

void BoundingSphere::BuildConstantBuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(BoundCBUFFER);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ThrowIfFailed(_device->CreateBuffer(&bufferDesc, NULL, _constantBuffer.GetAddressOf()));
}

void BoundingSphere::BuildRendererStates()
{
	// Set default and wireframe rasteriser states
	D3D11_RASTERIZER_DESC rasteriserDesc;
	rasteriserDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasteriserDesc.CullMode = D3D11_CULL_BACK;
	rasteriserDesc.FrontCounterClockwise = true;
	rasteriserDesc.DepthBias = 0;
	rasteriserDesc.SlopeScaledDepthBias = 0.0f;
	rasteriserDesc.DepthBiasClamp = 0.0f;
	rasteriserDesc.DepthClipEnable = true;
	rasteriserDesc.ScissorEnable = false;
	rasteriserDesc.MultisampleEnable = false;
	rasteriserDesc.AntialiasedLineEnable = false;
	ThrowIfFailed(_device->CreateRasterizerState(&rasteriserDesc, _defaultRasteriserState.GetAddressOf()));
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
