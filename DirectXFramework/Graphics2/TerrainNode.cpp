#include "TerrainNode.h"

// Completes all steps necessary for terrain to be rendered
bool TerrainNode::Initialise()
{
	LoadTerrainTextures();
	BuildWaterNormals();
	BuildGeometryBuffers();
	GenerateBlendMap();
	BuildBuffers();
	BuildShaders();
	BuildVertexLayout();
	BuildConstantBuffer();
    BuildRendererStates();

	return true;
}

void TerrainNode::Render()
{
	// Calculate the world x view x projection transformation
	XMMATRIX completeTransformation = XMLoadFloat4x4(&_worldTransformation) * DirectXFramework::GetDXFramework()->GetCamera()->GetViewMatrix() * DirectXFramework::GetDXFramework()->GetProjectionTransformation();

	// Update the constant buffer 
	CBUFFER cBuffer;
	cBuffer.CompleteTransformation = completeTransformation;
	cBuffer.WorldTransformation = XMLoadFloat4x4(&_worldTransformation);
	cBuffer.AmbientColor = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	cBuffer.LightVector = XMVector4Normalize(XMVectorSet(0.5f, -1.0f, -1.0f, 0.0f));
	cBuffer.LightColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMStoreFloat4(&cBuffer.CameraPosition, DirectXFramework::GetDXFramework()->GetCamera()->GetPosition());

	_deviceContext->VSSetShader(_vertexShader.Get(), 0, 0);
	_deviceContext->PSSetShader(_pixelShader.Get(), 0, 0);
	_deviceContext->IASetInputLayout(_layout.Get());

	// Now render the terrain
	UINT stride = sizeof(TerrainVertex);
	UINT offset = 0;
	_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	cBuffer.DiffuseCoefficient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	cBuffer.SpecularCoefficient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cBuffer.Shininess = 2.0f;
	cBuffer.Opacity = 1.0f;
	_deviceContext->UpdateSubresource(_constantBuffer.Get(), 0, 0, &cBuffer, 0, 0);
	_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	_deviceContext->PSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	_deviceContext->PSSetShaderResources(0, 1, _blendMapResourceView.GetAddressOf());
	_deviceContext->PSSetShaderResources(1, 1, _texturesResourceView.GetAddressOf());
	// Passes in current normal map, index uses %120 to ensure value is always between 0 and 120
	_deviceContext->PSSetShaderResources(2, 1, _waterNormalsResourceView[_normalMap % 120].GetAddressOf());
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_deviceContext->RSSetState(_defaultRasteriserState.Get());
	_deviceContext->DrawIndexed(static_cast<UINT>(_indices.size()), 0, 0);

	// Increment the animation frame to be used every 5 frames
	_frame++;
	if (_frame % 5 == 0)
	{
		_normalMap++;
	}
}

void TerrainNode::Shutdown()
{
}

void TerrainNode::BuildRendererStates()
{
	// Set default and wireframe rasteriser states
	D3D11_RASTERIZER_DESC rasteriserDesc;
	rasteriserDesc.FillMode = D3D11_FILL_SOLID;
	rasteriserDesc.CullMode = D3D11_CULL_BACK;
	rasteriserDesc.FrontCounterClockwise = false;
	rasteriserDesc.DepthBias = 0;
	rasteriserDesc.SlopeScaledDepthBias = 0.0f;
	rasteriserDesc.DepthBiasClamp = 0.0f;
	rasteriserDesc.DepthClipEnable = true;
	rasteriserDesc.ScissorEnable = false;
	rasteriserDesc.MultisampleEnable = false;
	rasteriserDesc.AntialiasedLineEnable = false;
	ThrowIfFailed(_device->CreateRasterizerState(&rasteriserDesc, _defaultRasteriserState.GetAddressOf()));
	rasteriserDesc.FillMode = D3D11_FILL_WIREFRAME;
	ThrowIfFailed(_device->CreateRasterizerState(&rasteriserDesc, _wireframeRasteriserState.GetAddressOf()));
}

// Loads height map values from file into a vector
bool TerrainNode::LoadHeightMap(wstring heightMapFilename)
{
	unsigned int mapSize = _numberOfXPoints * _numberOfZPoints;
	USHORT* rawFileValues = new USHORT[mapSize];

	std::ifstream inputHeightMap;
	inputHeightMap.open(heightMapFilename.c_str(), std::ios_base::binary);
	if (!inputHeightMap)
	{
		return false;
	}

	inputHeightMap.read((char*)rawFileValues, mapSize * 2);
	inputHeightMap.close();

	// Normalise BYTE values to the range 0.0f - 1.0f;
	for (unsigned int i = 0; i < mapSize; i++)
	{
		_heightValues.push_back((float)rawFileValues[i] / 65536);
	}
	delete[] rawFileValues;
	return true;
}

void TerrainNode::BuildGeometryBuffers()
{
	LoadHeightMap(_heightMapPath);

	float offsetU = 1.0f / _numberOfXPoints;
	float offsetV = 1.0f / _numberOfZPoints;

	// Loops through the grid
	for (int x = 0; x < _numberOfXPoints - 1; x++)
	{
		for (int z = 0; z < _numberOfZPoints - 1; z++)
		{
			// Randomizes UV values to reduce tiling effect on textures, not applied this to water as the normal maps I used are seemless and are meant to be tiled
			float U0 = RandomInRange(0.0f, 0.3f);
			float U1 = RandomInRange(0.7f, 1.0f);
			float V0 = RandomInRange(0.0f, 0.3f);
			float V1 = RandomInRange(0.7f, 1.0f);
			// Creates the vertices:		  Position         X                                    Y                                                                               Z                                     Normal                      TexCoord U  V     BlendMapTexCoord U           V                  WaterTexCoord
			_vertices.push_back(TerrainVertex(XMFLOAT3((float)(x * _spacing) + _terrainStart,       _heightValues[(x * _numberOfXPoints) + z + 1] * _numberOfXPoints,       (float)((z + 1) * _spacing) + _terrainStart), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(U0, V0), XMFLOAT2(x * offsetU,	    (z + 1) * offsetV), XMFLOAT2(0.0f, 0.0f))); // v1
			_vertices.push_back(TerrainVertex(XMFLOAT3((float)((x + 1) * _spacing) + _terrainStart, _heightValues[((x + 1) * _numberOfXPoints) + z + 1] * _numberOfXPoints, (float)((z + 1) * _spacing) + _terrainStart), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(U1, V0), XMFLOAT2((x + 1) * offsetU, (z + 1) * offsetV), XMFLOAT2(1.0f, 0.0f))); // v2
			_vertices.push_back(TerrainVertex(XMFLOAT3((float)(x * _spacing) + _terrainStart,       _heightValues[(x * _numberOfXPoints) + z] * _numberOfXPoints,           (float)(z * _spacing) + _terrainStart),       XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(U0, V1), XMFLOAT2(x * offsetU,		 z * offsetV),		XMFLOAT2(0.0f, 1.0f))); // v3
			_vertices.push_back(TerrainVertex(XMFLOAT3((float)((x + 1) * _spacing) + _terrainStart, _heightValues[((x + 1) * _numberOfXPoints) + z] * _numberOfXPoints,     (float)(z * _spacing) + _terrainStart),       XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(U1, V1), XMFLOAT2((x + 1) * offsetU,  z * offsetV),		XMFLOAT2(1.0f, 1.0f))); // v4

			UINT v1 = static_cast<UINT>(_vertices.size()) - 4;
			UINT v2 = v1 + 1;
			UINT v3 = v2 + 1;
			UINT v4 = v3 + 1;

			// Adds indices for the tow triangles in clockwise winding order
			_indices.push_back(v1);
			_indices.push_back(v2);
			_indices.push_back(v3);

			_indices.push_back(v3);
			_indices.push_back(v2);
			_indices.push_back(v4);
		}
	}	

	// Calculating normals
	int index = 0;
	// Amount of indices we have to move to reach the next column
	int multiplier = 4 * (_numberOfZPoints - 1);

	// Loops through grid
	for (int x = 0; x < _numberOfXPoints - 1; x++)
	{
		for (int z = 0; z < _numberOfZPoints - 1; z++)
		{
			// Getting face normal
			XMVECTOR v1 = XMLoadFloat3(&_vertices[index].Position);
			XMVECTOR v2 = XMLoadFloat3(&_vertices[index + 1].Position);
			XMVECTOR v3 = XMLoadFloat3(&_vertices[index + 2].Position);
			XMVECTOR faceNormal = XMVector3Cross(v2 - v1, v3 - v1);

			// Add face normal to total in this square
			DirectX::XMStoreFloat3(&_vertices[index].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index].Normal), faceNormal));
			DirectX::XMStoreFloat3(&_vertices[index + 1].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index + 1].Normal), faceNormal));
			DirectX::XMStoreFloat3(&_vertices[index + 2].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index + 2].Normal), faceNormal));
			DirectX::XMStoreFloat3(&_vertices[index + 3].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index + 3].Normal), faceNormal));

			// Add face normal to surrounding squares
			// Check if we are at the edge of the grid, without this we will get a vertex out of bounds error

			// Squares to the left
			if (x > 0)
			{
				// Below to the left
				if (z > 0)
				{
					DirectX::XMStoreFloat3(&_vertices[index - multiplier - 3].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index - multiplier - 3].Normal), faceNormal));
				}

				// Directly to the left
				DirectX::XMStoreFloat3(&_vertices[index - multiplier + 1].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index - multiplier + 1].Normal), faceNormal));
				DirectX::XMStoreFloat3(&_vertices[index - multiplier + 3].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index - multiplier + 3].Normal), faceNormal));

				// Above to the left
				if (z < _numberOfZPoints - 2)
				{
					DirectX::XMStoreFloat3(&_vertices[index - multiplier + 7].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index - multiplier + 7].Normal), faceNormal));
				}
			}

			// Squares to the right
			if (x < _numberOfXPoints - 2)
			{
				// Below to the right
				if (z > 0)
				{
					DirectX::XMStoreFloat3(&_vertices[index + multiplier - 4].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index + multiplier - 4].Normal), faceNormal));
				}

				// Directly to the right
				DirectX::XMStoreFloat3(&_vertices[index + multiplier].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index + multiplier].Normal), faceNormal));
				DirectX::XMStoreFloat3(&_vertices[index + multiplier + 2].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index + multiplier + 2].Normal), faceNormal));

				// Above to the right
				if (z < _numberOfZPoints - 2)
				{
					DirectX::XMStoreFloat3(&_vertices[index + multiplier + 6].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index + multiplier + 6].Normal), faceNormal));
				}
			}

			// Square below
			if (z > 0)
			{
				DirectX::XMStoreFloat3(&_vertices[index - 3].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index - 3].Normal), faceNormal));
				DirectX::XMStoreFloat3(&_vertices[index - 4].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index - 4].Normal), faceNormal));
			}

			// Square above
			if (z < _numberOfZPoints - 2)
			{
				DirectX::XMStoreFloat3(&_vertices[index + 6].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index + 6].Normal), faceNormal));
				DirectX::XMStoreFloat3(&_vertices[index + 7].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index + 7].Normal), faceNormal));
			}

			// Increment index by 4 as there are 4 vertices per square
			index += 4;
		}
	}

	// Normalise all vertex normals
	for (TerrainVertex& vertex : _vertices)
	{
		DirectX::XMStoreFloat3(&vertex.Normal, XMVector3Normalize(XMLoadFloat3(&vertex.Normal)));
	}
}

// Creates vertex and index buffers using values calculated in BuildGeometryBuffers
void TerrainNode::BuildBuffers()
{
	// Setup the structure that specifies how big the vertex 
	// buffer should be
	D3D11_BUFFER_DESC vertexBufferDescriptor;
	vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescriptor.ByteWidth = sizeof(TerrainVertex) * static_cast<UINT>(_vertices.size());
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

void TerrainNode::BuildShaders()
{
	DWORD shaderCompileFlags = 0;
#if defined( _DEBUG )
	shaderCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> compilationMessages = nullptr;

	//Compile vertex shader
	HRESULT hr = D3DCompileFromFile(L"Shaders\\TerrainShaders.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VShader", "vs_5_0",
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

	// Compile pixel shader
	hr = D3DCompileFromFile(L"Shaders\\TerrainShaders.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PShader", "ps_5_0",
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
}

void TerrainNode::BuildVertexLayout()
{
	// Create the vertex input layout. This tells DirectX the format
	// of each of the vertices we are sending to it.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Tex coord
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Blend map tex coord
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 } // Water tex coord
	};

	ThrowIfFailed(_device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), _vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), _layout.GetAddressOf()));
}

void TerrainNode::BuildConstantBuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(CBUFFER);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ThrowIfFailed(_device->CreateBuffer(&bufferDesc, NULL, _constantBuffer.GetAddressOf()));
}

void TerrainNode::BuildWaterNormals()
{
	// Load water normal maps
	for (int i = 1; i <= 120; i++)
	{
		// File names are integers padded with zero's to be 4 characters long
		wstring texturePath = L"" + to_wstring(i);
		while (texturePath.size() < 4)
		{
			texturePath = L"0" + texturePath;
		}
		texturePath = L"Water\\" + texturePath + L".png";
		ComPtr<ID3D11ShaderResourceView> waterNormalMap;
		// Load water normal map
		ThrowIfFailed(CreateWICTextureFromFile(_device.Get(),
			_deviceContext.Get(),
			texturePath.c_str(),
			nullptr,
			waterNormalMap.GetAddressOf()
		));
		// Add it to the array
		_waterNormalsResourceView[i - 1] = waterNormalMap;
	}
}

void TerrainNode::LoadTerrainTextures()
{
	// Change the paths below as appropriate for your use
	wstring terrainTextureNames[5] = { L"Terrain\\grass.dds", L"Terrain\\darkdirt.dds", L"Terrain\\stone.dds", L"Terrain\\lightdirt.dds", L"Terrain\\snow.dds" };

	// Load the textures from the files
	ComPtr<ID3D11Resource> terrainTextures[5];
	for (int i = 0; i < 5; i++)
	{
		ThrowIfFailed(CreateDDSTextureFromFileEx(_device.Get(),
			_deviceContext.Get(),
			terrainTextureNames[i].c_str(),
			0,
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_SHADER_RESOURCE,
			0,
			0,
			false,
			terrainTextures[i].GetAddressOf(),
			nullptr
		));
	}
	// Now create the Texture2D arrary.  We assume all textures in the
	// array have the same format and dimensions

	D3D11_TEXTURE2D_DESC textureDescription;
	ComPtr<ID3D11Texture2D> textureInterface;
	terrainTextures[0].As<ID3D11Texture2D>(&textureInterface);
	textureInterface->GetDesc(&textureDescription);

	D3D11_TEXTURE2D_DESC textureArrayDescription;
	textureArrayDescription.Width = textureDescription.Width;
	textureArrayDescription.Height = textureDescription.Height;
	textureArrayDescription.MipLevels = textureDescription.MipLevels;
	textureArrayDescription.ArraySize = 5;
	textureArrayDescription.Format = textureDescription.Format;
	textureArrayDescription.SampleDesc.Count = 1;
	textureArrayDescription.SampleDesc.Quality = 0;
	textureArrayDescription.Usage = D3D11_USAGE_DEFAULT;
	textureArrayDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureArrayDescription.CPUAccessFlags = 0;
	textureArrayDescription.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> textureArray = 0;
	ThrowIfFailed(_device->CreateTexture2D(&textureArrayDescription, 0, textureArray.GetAddressOf()));

	// Copy individual texture elements into texture array.

	for (UINT i = 0; i < 5; i++)
	{
		// For each mipmap level...
		for (UINT mipLevel = 0; mipLevel < textureDescription.MipLevels; mipLevel++)
		{
			_deviceContext->CopySubresourceRegion(textureArray.Get(),
				D3D11CalcSubresource(mipLevel, i, textureDescription.MipLevels),
				NULL,
				NULL,
				NULL,
				terrainTextures[i].Get(),
				mipLevel,
				nullptr
			);
		}
	}

	// Create a resource view to the texture array.
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDescription;
	viewDescription.Format = textureArrayDescription.Format;
	viewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDescription.Texture2DArray.MostDetailedMip = 0;
	viewDescription.Texture2DArray.MipLevels = textureArrayDescription.MipLevels;
	viewDescription.Texture2DArray.FirstArraySlice = 0;
	viewDescription.Texture2DArray.ArraySize = 5;

	ThrowIfFailed(_device->CreateShaderResourceView(textureArray.Get(), &viewDescription, _texturesResourceView.GetAddressOf()));
}

float TerrainNode::Absolute(float value)
{
	return sqrt(pow(value, 2));
}

float TerrainNode::RandomInRange(float min, float max)
{
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

void TerrainNode::GenerateBlendMap()
{
	DWORD* blendMap = new DWORD[_numberOfXPoints * _numberOfZPoints];
	DWORD* blendMapPtr = blendMap;
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
	
	// Heights at which to introduce certain textures
	float fadeToGrassHeight = 0.03f;
	float grassHeight = 0.045f;
	float stoneHeight = 0.4f;
	float snowHeight;

	// Looping through the grid
	for (int i = 0; i < _numberOfXPoints; i++)
	{
		for (int j = 0; j < _numberOfZPoints; j++)
		{
			r = 0;
			g = 0;
			b = 0;
			a = 0;

			// Calculate the appropriate blend colour for the 
			// current location in the blend map.  This has been
			// left as an exercise for you.  You need to calculate
			// appropriate values for the r, g, b and a values (each
			// between 0 and 255). The code below combines these
			// into a DWORD (32-bit value) and stores it in the blend map.

			float height = _heightValues[(j * _numberOfXPoints) + i];
			float averageSlope = 0.0f;
			// Snow height is randomized giving a nice draping effect
			snowHeight = RandomInRange(0.5f, 0.6f);

			// Caluclates average slope amount for the square
			if (i != _numberOfXPoints - 1 && j != _numberOfZPoints - 1)
			{
				float height2 = _heightValues[((j + 1) * _numberOfXPoints) + i + 1];
				float height3 = _heightValues[(j * _numberOfXPoints) + i + 1];
				float height4 = _heightValues[((j + 1) * _numberOfXPoints) + i];
				averageSlope = (Absolute(height - height2) + Absolute(height - height3) + Absolute(height - height4)) / 3;
			}

			if (height < grassHeight)
			{
				if (height < fadeToGrassHeight)
				{
					// Use full light dirt texture
					b = 255;
				}
				else
				{
					// Fade out light dirt texture by height
					b = 255 - (255 * ((height - fadeToGrassHeight) / (grassHeight - fadeToGrassHeight)));
				}
			}
			else
			{
				if (averageSlope >= 0.006)
				{
					// Add rocks, used rock texture mixed with black snow texture to make it darker since I used normal rock texture for snow
					g = 255;
					a = 100;
				}
				if (height >= snowHeight)
				{
					// Snow texture is black? Used rock for snow instead
					g = 255;
					a = 0;
				}
			}
			
			DWORD mapValue = (a << 24) + (b << 16) + (g << 8) + r;
			*blendMapPtr++ = mapValue;
		}
	}
	D3D11_TEXTURE2D_DESC blendMapDescription;
	blendMapDescription.Width = _numberOfXPoints;
	blendMapDescription.Height = _numberOfZPoints;
	blendMapDescription.MipLevels = 1;
	blendMapDescription.ArraySize = 1;
	blendMapDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	blendMapDescription.SampleDesc.Count = 1;
	blendMapDescription.SampleDesc.Quality = 0;
	blendMapDescription.Usage = D3D11_USAGE_DEFAULT;
	blendMapDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	blendMapDescription.CPUAccessFlags = 0;
	blendMapDescription.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA blendMapInitialisationData;
	blendMapInitialisationData.pSysMem = blendMap;
	blendMapInitialisationData.SysMemPitch = 4 * _numberOfXPoints;

	ComPtr<ID3D11Texture2D> blendMapTexture;
	ThrowIfFailed(_device->CreateTexture2D(&blendMapDescription, &blendMapInitialisationData, blendMapTexture.GetAddressOf()));

	// Create a resource view to the texture array.
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDescription;
	viewDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	viewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDescription.Texture2D.MostDetailedMip = 0;
	viewDescription.Texture2D.MipLevels = 1;

	ThrowIfFailed(_device->CreateShaderResourceView(blendMapTexture.Get(), &viewDescription, _blendMapResourceView.GetAddressOf()));
	delete[] blendMap;
}

// Returns the height at a given point
float TerrainNode::GetHeightAtPoint(float x, float z)
{
	// Get the cell we are in
	int cellX = (int)((x - _terrainStart) / _spacing);
	int cellZ = (int)((z - _terrainStart) / _spacing);

	// Get point at start of this cell
	float cellStartX = static_cast<float>(_terrainStart + (cellX * _spacing));
	float cellStartZ = static_cast<float>(_terrainStart + (cellZ * _spacing));

	XMFLOAT3 v0 = XMFLOAT3(cellStartX, _heightValues[(cellX * _numberOfXPoints) + cellZ + 1] * _numberOfXPoints, cellStartZ + 10);
	XMFLOAT3 v2 = XMFLOAT3(cellStartX + 10, _heightValues[((cellX + 1) * _numberOfXPoints) + cellZ] * _numberOfXPoints, cellStartZ);
	XMFLOAT3 other;

	// Determine which triangle we are in
	float dx = Absolute(x - cellStartX);
	float dz = Absolute(z - cellStartZ);
	if (dx > dz)
	{
		other = XMFLOAT3(cellStartX + 10, _heightValues[((cellX + 1) * _numberOfXPoints) + cellZ + 1] * _numberOfXPoints, cellStartZ + 10);
	}
	else
	{
		other = XMFLOAT3(cellStartX, _heightValues[(cellX * _numberOfXPoints) + cellZ] * _numberOfXPoints, cellStartZ);
	}

	// Get the normal of the traingle we are in
	XMVECTOR faceNormal = XMVector3Cross(XMLoadFloat3(&other) - XMLoadFloat3(&v0), XMLoadFloat3(&v2) - XMLoadFloat3(&v0));
	faceNormal = XMVector3Normalize(faceNormal);
	XMFLOAT3 N;
	DirectX::XMStoreFloat3(&N, faceNormal);

	// Calculate y from this
	float y = v0.y + ((N.x * dx + N.z * dz) / -N.y);

	return y;
}

// Returns true if the given node is colliding with the terrain at all
bool TerrainNode::NodeHitFloor(shared_ptr<SceneNode> node)
{
	shared_ptr<BoundingSphere> bound = dynamic_pointer_cast<BoundingSphere>(node->GetBoundingVolume());

	// Loops through all sub bounds
	for (shared_ptr<BoundingSphere> subBound : bound->GetSubBounds())
	{
		float radius = subBound->GetRadius();
		XMFLOAT3 centrePos = subBound->GetCentrePos();
		// Applies world transform to the centre pos
		XMFLOAT4X4 centreWorldPos;
		XMStoreFloat4x4(&centreWorldPos, XMMatrixTranslation(centrePos.x, centrePos.y, centrePos.z) * XMLoadFloat4x4(&bound->GetCombinedWorldTransformation()));
		centrePos = XMFLOAT3(centreWorldPos._41, centreWorldPos._42, centreWorldPos._43);

		// Checks all points that are within the x and z area of the bound
		for (float x = centrePos.x - radius; x < centrePos.x + radius; x += 1)
		{
			for (float z = centrePos.z - radius; z < centrePos.z + radius; z += 1)
			{
				// Gets the height of the terrain at this point
				float heightOfTerrain = GetHeightAtPoint(x, z);
				float centreToTerrain = BoundingSphere::Distance(centrePos, XMFLOAT3(x, heightOfTerrain, z));
				// If the distance from this point to the centre of the sphere is less than or equal to the radius then the node is colliding
				if (centreToTerrain <= radius)
				{
					return true;
				}
			}
		}
	}
	return false;
}
