#include <../EngineShader/RenderBaseData.hlsli>

struct FEngineVertex
{
	float4 POSITION : POSITION;
	float4 UV : TEXCOORD;
};

struct VertexShaderOutPut
{
	float4 SVPOSITION : SV_POSITION;
	float4 UV : TEXCOORD;
};

cbuffer MyData : register(b9)
{
	float4 ScreenScale;
};

VertexShaderOutPut ShrinkEffect_VS(FEngineVertex _Vertex)
{
	VertexShaderOutPut OutPut;
	OutPut.UV = _Vertex.UV;
	OutPut.SVPOSITION = _Vertex.POSITION;
	
	float ATime = pow(AccTime, 3);
	
	if (0.5f < OutPut.UV.x)
	{
		OutPut.UV.x += ATime;
	}
	else if (0.5f > OutPut.UV.x)
	{
		OutPut.UV.x -= ATime;
	}
	
    if (0.5f < OutPut.UV.y)
    {
        OutPut.UV.y += ATime;
    }
    else if (0.5f > OutPut.UV.y)
    {
        OutPut.UV.y -= ATime;
    }
	
	return OutPut;
}

Texture2D ImageTexture : register(t1);
SamplerState ImageSampler : register(s1);

float4 ShrinkEffect_PS(VertexShaderOutPut _Vertex) : SV_Target0
{
    if (_Vertex.UV.x < 0 || _Vertex.UV.x > 1.0f)
	{
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
    if (_Vertex.UV.y < 0 || _Vertex.UV.y > 1.0f)
	{
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	float4 Color = ImageTexture.Sample(ImageSampler, _Vertex.UV.xy);
	return Color;
};
