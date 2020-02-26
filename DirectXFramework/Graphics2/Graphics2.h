#pragma once
#include "DirectXFramework.h"
#include "Cube.h"
#include "TerrainNode.h"

class Graphics2 : public DirectXFramework
{
public:
	void CreateSceneGraph();
	void UpdateSceneGraph();
private:
	float _rotation = 0;
	wchar_t* _brickTexture = L"Textures/Bricks.png";
	wchar_t* _woodTexture = L"Textures/Wood.png";
	wchar_t* _concreteTexture = L"Textures/Concrete.png";
	wchar_t* _woodblockTexture = L"Textures/woodblock.bmp";
};

