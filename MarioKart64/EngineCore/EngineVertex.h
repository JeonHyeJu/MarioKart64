#pragma once
#include <EngineBase/EngineMath.h>
#include "EngineInputLayOutInfo.h"

struct FEngineVertex
{
	friend class EngineVertexInit;
	static UEngineInputLayOutInfo Info;
	ENGINEAPI static UEngineInputLayOutInfo& GetInfo();

	float4 POSITION;
	float4 TEXCOORD;
	float4 COLOR;
};
