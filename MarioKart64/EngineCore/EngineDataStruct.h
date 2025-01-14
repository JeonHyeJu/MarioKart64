#pragma once
#include <EngineBase/EngineMath.h>

struct FUVValue
{
	float4 PlusUVValue;
};


struct FResultColor
{
public:
	float4 PlusColor;
	float4 MulColor;
};

struct FSpriteData
{
	float4 CuttingPos = { 0.0f, 0.0f };
	float4 CuttingSize = { 1.0f, 1.0f };
	float4 Pivot = { 0.5f, 0.5f };
};

