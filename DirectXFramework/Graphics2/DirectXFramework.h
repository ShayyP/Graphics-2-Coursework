#pragma once
#include <vector>
#include "Framework.h"
#include "DirectXCore.h"
#include "SceneGraph.h"
#include "WICTextureLoader.h"
#include "ResourceManager.h"

struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 TextureCoordinate;
};

struct CBUFFER
{
	XMMATRIX    CompleteTransformation;
	XMMATRIX	WorldTransformation;
	XMFLOAT4	CameraPosition;
	XMVECTOR    LightVector;
	XMFLOAT4    LightColor;
	XMFLOAT4    AmbientColor;
	XMFLOAT4    DiffuseCoefficient;
	XMFLOAT4	SpecularCoefficient;
	float		Shininess;
	float		Opacity;
	float       Padding[2];
};

class DirectXFramework : public Framework
{
public:
	DirectXFramework();
	DirectXFramework(unsigned int width, unsigned int height);

	virtual void CreateSceneGraph();
	virtual void UpdateSceneGraph();

	bool Initialise();
	void Update();
	void Render();
	void OnResize(WPARAM wParam);
	void Shutdown();

	static DirectXFramework *			GetDXFramework();

	inline SceneGraphPointer			GetSceneGraph() { return _sceneGraph; }
	inline ComPtr<ID3D11Device>			GetDevice() { return _device; }
	inline ComPtr<ID3D11DeviceContext>	GetDeviceContext() { return _deviceContext; }

	XMMATRIX							GetViewTransformation();
	XMMATRIX							GetProjectionTransformation();

	void								SetBackgroundColour(XMFLOAT4 backgroundColour);

	inline shared_ptr<ResourceManager> GetResourceManager() { return _resourceManager; }

private:
	ComPtr<ID3D11Device>				_device;
	ComPtr<ID3D11DeviceContext>			_deviceContext;
	ComPtr<IDXGISwapChain>				_swapChain;
	ComPtr<ID3D11Texture2D>				_depthStencilBuffer;
	ComPtr<ID3D11RenderTargetView>		_renderTargetView;
	ComPtr<ID3D11DepthStencilView>		_depthStencilView;

	D3D11_VIEWPORT						_screenViewport;

	shared_ptr<ResourceManager> _resourceManager;

	// Our vectors and matrices.  Note that we cannot use
	// XMVECTOR and XMMATRIX for class variables since they need
	// to be aligned on 16-byte boundaries and the compiler cannot
	// guarantee this for class variables

	// For now, we are storing our camera vectors and matrix here.
	// We will move it to a separate Camera class later
	XMFLOAT4							_eyePosition;
	XMFLOAT4							_focalPointPosition;
	XMFLOAT4							_upVector;

	XMFLOAT4X4							_viewTransformation;
	XMFLOAT4X4							_projectionTransformation;

	SceneGraphPointer					_sceneGraph;

	float							    _backgroundColour[4];

	bool GetDeviceAndSwapChain();
};

