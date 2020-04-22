#pragma once
#include "DirectXFramework.h"
#include "DDSTextureLoader.h"

struct TerrainVertex
{
	TerrainVertex(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT2 texCoord, XMFLOAT2 blendMapTexCoord)
	{
		Position = position;
		Normal = normal;
		TexCoord = texCoord;
		BlendMapTexCoord = blendMapTexCoord;
	}
	TerrainVertex()
	{
		Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		TexCoord = XMFLOAT2(0.0f, 0.0f);
		BlendMapTexCoord = XMFLOAT2(0.0f, 0.0f);
	}
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord;
	XMFLOAT2 BlendMapTexCoord;
};

class TerrainNode : public SceneNode
{
public:
	TerrainNode(wstring name, wstring heightMapPath) : SceneNode(name)
	{
		_heightMapPath = heightMapPath;
	}

	virtual bool Initialise();
	virtual void Render();
	virtual void Shutdown();

	void BuildRendererStates();

	bool LoadHeightMap(wstring heightMapFilename);

	float Random(float min, float max);

	void BuildGeometryBuffers();
	void BuildShaders();
	void BuildVertexLayout();
	void BuildConstantBuffer();
	void BuildTexture();

	void LoadTerrainTextures();
	float Absolute(float value);
	float RandomInRange(float min, float max);
	void GenerateBlendMap();

	float GetHeightAtPoint(float x, float z);

private:
	wstring _heightMapPath;
	vector<TerrainVertex> _vertices;
	vector<UINT> _indices;
	ComPtr<ID3D11ShaderResourceView> _texture;
	ComPtr<ID3D11Device> _device = DirectXFramework::GetDXFramework()->GetDevice();
	ComPtr<ID3D11DeviceContext> _deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3DBlob> _vertexShaderByteCode = nullptr;
	ComPtr<ID3DBlob> _pixelShaderByteCode = nullptr;
	ComPtr<ID3D11InputLayout> _layout;
	ComPtr<ID3D11Buffer> _constantBuffer;
	ComPtr<ID3D11RasterizerState> _defaultRasteriserState;
	ComPtr<ID3D11RasterizerState> _wireframeRasteriserState;
	ComPtr<ID3D11ShaderResourceView> _texturesResourceView;
	ComPtr<ID3D11ShaderResourceView> _blendMapResourceView;
	vector<float> _heightValues;
	unsigned int _numberOfXPoints = 1024;
	unsigned int _numberOfZPoints = 1024;
	int _terrainStart = -5120;
	int _spacing = 10;
};

