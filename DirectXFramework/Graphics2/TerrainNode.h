#pragma once
#include "DirectXFramework.h"

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

	void BuildGeometryBuffers();
	void BuildShaders();
	void BuildVertexLayout();
	void BuildConstantBuffer();
	void BuildTexture();

private:
	wstring _heightMapPath;
	vector<VERTEX> _vertices;
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
};

