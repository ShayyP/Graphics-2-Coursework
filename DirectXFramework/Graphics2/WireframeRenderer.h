#pragma once
#include "DirectXFramework.h"
#include "ResourceManager.h"

struct WireframeCBUFFER
{
	XMMATRIX CompleteTransformation;
};

class WireframeRenderer
{
public:
	WireframeRenderer();

	void Render(vector<XMFLOAT3> vertices, vector<UINT> indices, XMFLOAT4X4 completeWorldTransformation);

	void BuildShaders();
	void BuildRendererStates();
	void BuildVertexLayout();
	void BuildConstantBuffer();

private:
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
	ComPtr<ID3D11RasterizerState> _wireframeRasteriserState;
};

