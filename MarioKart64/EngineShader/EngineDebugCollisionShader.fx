#include "Transform.hlsli"

struct EngineVertex
{
	float4 POSITION : POSITION;
};

struct VertexShaderOutPut
{
	float4 SVPOSITION : SV_POSITION;
};

VertexShaderOutPut Debug_VS(EngineVertex _Vertex)
{
	VertexShaderOutPut OutPut;
	OutPut.SVPOSITION = mul(_Vertex.POSITION, WVP);
	return OutPut;
}

cbuffer OutColor : register(b0)
{
	float4 Color;
};

float4 PixelToWorld_PS(VertexShaderOutPut _Vertex) : SV_Target0
{
	return Color;
};
