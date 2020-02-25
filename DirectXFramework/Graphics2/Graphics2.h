#pragma once
#include "DirectXFramework.h"
#include "Cube.h"

class Graphics2 : public DirectXFramework
{
public:
	void CreateSceneGraph();
	XMFLOAT4X4* RotateAround(XMFLOAT4X4* WorldTransform, float angle);
	void UpdateSceneGraph();
private:
	float _rotation = 0;
	wchar_t* _brickTexture = L"Textures/Bricks.png";
	wchar_t* _woodTexture = L"Textures/Wood.png";
	wchar_t* _concreteTexture = L"Textures/Concrete.png";
	wchar_t* _woodblockTexture = L"Textures/woodblock.bmp";
};

