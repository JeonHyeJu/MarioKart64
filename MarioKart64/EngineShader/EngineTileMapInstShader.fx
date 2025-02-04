#include "Transform.hlsli"

struct EngineVertex
{
	float4 POSITION : POSITION;
	float4 UV : TEXCOORD;
	float4 COLOR : COLOR;
	uint iInstance : SV_InstanceID;
};

struct VertexShaderOutPut
{
	float4 SVPOSITION : SV_POSITION;
	float4 UV : TEXCOORD;
	float4 COLOR : COLOR;
	uint iInstance : SV_InstanceID;
};

struct FSpriteData
{
	float4 CuttingPos;
	float4 CuttingSize;
	float4 Pivot;
};

StructuredBuffer<FSpriteData> SpriteDataBuffer : register(t1);

VertexShaderOutPut TileMapInst_VS(EngineVertex _Vertex)
{
	VertexShaderOutPut OutPut;
	
	int Index = _Vertex.iInstance;
	OutPut.iInstance = Index;
	
	_Vertex.POSITION.x += (1.0f - SpriteDataBuffer[Index].Pivot.x) - 0.5f;
	_Vertex.POSITION.y += (1.0f - SpriteDataBuffer[Index].Pivot.y) - 0.5f;
	
	OutPut.SVPOSITION = mul(_Vertex.POSITION, TransformBuffer[Index].WVP);
	
	OutPut.UV = _Vertex.UV;
	OutPut.UV.x = (_Vertex.UV.x * SpriteDataBuffer[Index].CuttingSize.x) + SpriteDataBuffer[Index].CuttingPos.x;
	OutPut.UV.y = (_Vertex.UV.y * SpriteDataBuffer[Index].CuttingSize.y) + SpriteDataBuffer[Index].CuttingPos.y;
	
	OutPut.COLOR = _Vertex.COLOR;
	return OutPut;
}

Texture2D TileMapTex : register(t0);
SamplerState ImageSampler : register(s0);

struct FResultColor 
{
	float4 PlusColor;
	float4 MulColor;
};

StructuredBuffer<FResultColor> ColorDataBuffer : register(t1);

float4 TileMapInst_PS(VertexShaderOutPut _Vertex) : SV_Target0
{
	float4 Color = TileMapTex.Sample(ImageSampler, _Vertex.UV.xy);

	if (0.0f >= Color.a)
	{
		clip(-1);
	}
	
	Color += ColorDataBuffer[_Vertex.iInstance].PlusColor;
	Color *= ColorDataBuffer[_Vertex.iInstance].MulColor;
	return Color;
};
