#pragma once
#include "DirectXFramework.h"
#include "DDSTextureLoader.h"
#include "BoundingSphere.h"
#include "Structs.h"
#include <fstream>

// Scene node that represents terrain. Handles generation from a height map, textures and some collision
class TerrainNode : public SceneNode
{
public:
	TerrainNode(wstring name, wstring heightMapPath) : SceneNode(name) { _heightMapPath = heightMapPath; }

	virtual bool Initialise();
	virtual void Render();
	virtual void Shutdown();

	void BuildRendererStates();

	bool LoadHeightMap(wstring heightMapFilename);

	void BuildGeometryBuffers();
	void BuildBuffers();
	void BuildShaders();
	void BuildVertexLayout();
	void BuildConstantBuffer();
	void BuildWaterNormals();

	void LoadTerrainTextures();
	float Absolute(float value);
	float RandomInRange(float min, float max);
	void GenerateBlendMap();

	float GetHeightAtPoint(float x, float z);
	bool NodeHitFloor(shared_ptr<SceneNode> node);

private:
	wstring							 _heightMapPath;
	vector<TerrainVertex>			 _vertices;
	vector<UINT>					 _indices;
	ComPtr<ID3D11ShaderResourceView> _texture;
	ComPtr<ID3D11Device>			 _device = DirectXFramework::GetDXFramework()->GetDevice();
	ComPtr<ID3D11DeviceContext>		 _deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();
	ComPtr<ID3D11Buffer>			 _vertexBuffer;
	ComPtr<ID3D11Buffer>			 _indexBuffer;
	ComPtr<ID3D11VertexShader>		 _vertexShader;
	ComPtr<ID3D11PixelShader>		 _pixelShader;
	ComPtr<ID3DBlob>				 _vertexShaderByteCode = nullptr;
	ComPtr<ID3DBlob>				 _pixelShaderByteCode = nullptr;
	ComPtr<ID3D11InputLayout>		 _layout;
	ComPtr<ID3D11Buffer>			 _constantBuffer;
	ComPtr<ID3D11RasterizerState>    _defaultRasteriserState;
	ComPtr<ID3D11RasterizerState>    _wireframeRasteriserState;
	ComPtr<ID3D11ShaderResourceView> _texturesResourceView;
	ComPtr<ID3D11ShaderResourceView> _blendMapResourceView;
	ComPtr<ID3D11ShaderResourceView> _waterNormalsResourceView[120];
	vector<float>					 _heightValues;
	int								 _numberOfXPoints = 1024;
	int								 _numberOfZPoints = 1024;
	int								 _terrainStart = -5120;
	int								 _spacing = 10;
	int								 _normalMap = 0;
	int								 _frame = 0;
};

