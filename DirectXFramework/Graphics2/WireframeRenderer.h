#pragma once
#include <vector>
#include "DirectXFramework.h"
#include "Structs.h"

using namespace std;

// Renderer class for wireframe nodes. Only handles rendering (no updating or generating vertices)
class WireframeRenderer : public enable_shared_from_this<WireframeRenderer>
{
public:
	WireframeRenderer() { _numberOfIndices = 0; }
	void Initialise(vector<WireframeVertex> vertices, vector<UINT> indices);
	void Render(XMMATRIX worldTransform);

private:
	void BuildGeometryBuffers(vector<WireframeVertex> vertices, vector<UINT> indices);
	void BuildShaders();
	void BuildVertexLayout();
	void BuildConstantBuffer();
	void BuildRendererStates();

	UINT						  _numberOfIndices;
	ComPtr<ID3D11Buffer>		  _vertexBuffer;
	ComPtr<ID3D11Buffer>		  _indexBuffer;

	ComPtr<ID3D11Device>		  _device = DirectXFramework::GetDXFramework()->GetDevice();
	ComPtr<ID3D11DeviceContext>	  _deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();

	ComPtr<ID3D11VertexShader>	  _vertexShader;
	ComPtr<ID3D11PixelShader>	  _pixelShader;
	ComPtr<ID3DBlob>			  _vertexShaderByteCode = nullptr;
	ComPtr<ID3DBlob>		      _pixelShaderByteCode = nullptr;
	ComPtr<ID3D11InputLayout>	  _layout;
	ComPtr<ID3D11Buffer>		  _constantBuffer;
	ComPtr<ID3D11RasterizerState> _rasteriserState;
};

