#pragma once
#include "DirectXCore.h"

struct WireframeVertex
{
	XMFLOAT3 Position;
	XMFLOAT4 Colour;
};

struct WireframeCBUFFER
{
	XMMATRIX CompleteTransformation;
};