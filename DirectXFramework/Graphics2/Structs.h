#pragma once
#include "DirectXCore.h"

// Defines a number of structs that are used across the solution. Some have constructors where needed

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

struct BasicCBUFFER
{
	XMMATRIX CompleteTransformation;
};

struct Vertex
{
	Vertex(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT2 texCoord)
	{
		Position = position;
		Normal = normal;
		TextureCoordinate = texCoord;
	}
	Vertex()
	{
		Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		TextureCoordinate = XMFLOAT2(0.0f, 0.0f);
	}
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 TextureCoordinate;
};

struct TerrainVertex
{
	TerrainVertex(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT2 texCoord, XMFLOAT2 blendMapTexCoord, XMFLOAT2 waterTexCoord)
	{
		Position = position;
		Normal = normal;
		TexCoord = texCoord;
		BlendMapTexCoord = blendMapTexCoord;
		WaterTexCoord = waterTexCoord;
	}
	TerrainVertex()
	{
		Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		TexCoord = XMFLOAT2(0.0f, 0.0f);
		BlendMapTexCoord = XMFLOAT2(0.0f, 0.0f);
		WaterTexCoord = XMFLOAT2(0.0f, 0.0f);
	}
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord;
	XMFLOAT2 BlendMapTexCoord;
	XMFLOAT2 WaterTexCoord;
};

struct SkyVertex
{
	XMFLOAT3 Position;
};

struct WireframeVertex
{
	XMFLOAT3 Position;
	XMFLOAT4 Colour;
};
