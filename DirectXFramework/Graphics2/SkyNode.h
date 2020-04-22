#pragma once
#include "DirectXFramework.h"
#include "DDSTextureLoader.h"

struct SkyVertex
{
	XMFLOAT3 Position;
};

struct SkyCBUFFER
{
	XMMATRIX CompleteTransformation;
};

class SkyNode : public SceneNode
{
public:
	SkyNode(wstring name, wstring texturePath, float radius) : SceneNode(name)
	{
		_texturePath = texturePath;
		_radius = radius;
	}

	virtual bool Initialise();
	virtual void Render();
	virtual void Shutdown();

	void CreateSphere(float radius, size_t tessellation);

	void BuildRendererStates();
	void BuildDepthStencilState();

	void BuildTexture();

	void BuildShaders();
	void BuildVertexLayout();
	void BuildConstantBuffer();

private:
	wstring _texturePath;
	float _radius;
	vector<SkyVertex> _vertices;
	vector<UINT> _indices;
	unsigned int _numberOfVertices;
	unsigned int _numberOfIndices;
	ComPtr<ID3D11ShaderResourceView> _texture;
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;

	ComPtr<ID3D11Device> _device = DirectXFramework::GetDXFramework()->GetDevice();
	ComPtr<ID3D11DeviceContext> _deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();

	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3DBlob> _vertexShaderByteCode = nullptr;
	ComPtr<ID3DBlob> _pixelShaderByteCode = nullptr;
	ComPtr<ID3D11InputLayout> _layout;
	ComPtr<ID3D11Buffer> _constantBuffer;
	ComPtr<ID3D11RasterizerState>   _defaultRasteriserState;
	ComPtr<ID3D11RasterizerState>   _noCullRasteriserState;

	ComPtr<ID3D11DepthStencilState>	 _stencilState;
};

