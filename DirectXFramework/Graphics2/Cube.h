#pragma once

#include "SceneNode.h"
#include "DirectXFramework.h"

class Cube : public SceneNode
{
public:
	Cube() : SceneNode(L"Cube")
	{
		_texturePath = L"Wood.png";
	}

	Cube(wstring name, wchar_t* texturePath) : SceneNode(name) 
	{
		_texturePath = texturePath;
	};

	virtual bool Initialise();
	virtual void Render();
	virtual void Shutdown();

	void BuildGeometryBuffers();
	void BuildShaders();
	void BuildVertexLayout();
	void BuildConstantBuffer();
	void BuildTexture();

private:
	wchar_t* _texturePath;
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
};

