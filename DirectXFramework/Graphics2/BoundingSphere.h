#pragma once
#include "BoundingVolume.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "DirectXFramework.h"

using namespace std;

class BoundingSphere : public BoundingVolume
{
public:
	BoundingSphere(shared_ptr<Mesh> mesh);
	BoundingSphere(shared_ptr<SubMesh> subMesh);
	static float Distance(XMFLOAT3 point1, XMFLOAT3 point2);
	void BuildSphere(float radius, XMFLOAT3 centrePos);
	void MergeSubBounds();

	virtual void Initialise();
	virtual void Render();
	virtual void Update(FXMMATRIX& worldTransformation) { XMStoreFloat4x4(&_combinedWorldTransformation, worldTransformation); }

	void BuildGeometryBuffers();
	void BuildShaders();
	void BuildVertexLayout();
	void BuildConstantBuffer();
	void BuildRendererStates();

	float GetRadius() { return _radius; }
	XMFLOAT3 GetCentrePos() { return _centrePos; }
	virtual vector<shared_ptr<BoundingSphere>> GetSubBounds() { return _subBounds; }

	virtual bool IsIntersecting(shared_ptr<BoundingVolume> otherVolume);
	virtual float IsIntersectingRay(XMVECTOR origin, XMVECTOR direction);

private:
	shared_ptr<Mesh> _mesh = nullptr;
	shared_ptr<SubMesh> _subMesh = nullptr;
	vector<shared_ptr<BoundingSphere>> _subBounds;

	float _radius;
	XMFLOAT3 _centrePos;

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
};

