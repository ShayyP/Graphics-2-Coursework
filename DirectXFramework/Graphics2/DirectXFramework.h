#pragma once
#include <vector>
#include "Framework.h"
#include "DirectXCore.h"
#include "SceneGraph.h"
#include "WICTextureLoader.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "GamePadController.h"

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
	inline shared_ptr<ResourceManager>  GetResourceManager() { return _resourceManager; }
	inline shared_ptr<Camera>           GetCamera() { return _camera; }
	inline shared_ptr<GamePadController> GetController() { return _controller; }
	XMMATRIX							GetProjectionTransformation();

	void								SetBackgroundColour(XMFLOAT4 backgroundColour);

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

	shared_ptr<Camera>                  _camera;
	shared_ptr<GamePadController>       _controller;

	XMFLOAT4X4							_projectionTransformation;

	SceneGraphPointer					_sceneGraph;

	float							    _backgroundColour[4];

	bool GetDeviceAndSwapChain();
};

