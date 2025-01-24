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

VertexShaderOutPut ShrinkEffect_VS(FEngineVertex _Vertex)
{
	VertexShaderOutPut OutPut;
	OutPut.UV = _Vertex.UV;
	OutPut.SVPOSITION = _Vertex.POSITION;
	
	float ATime = pow(AccTime, 3);
    const float INIT_S = -4.f;
    const float INIT_L = 5.f;
	
	if (0.5f < OutPut.UV.x)
	{
		OutPut.UV.x += ATime;
        if (OutPut.UV.x > INIT_L)
        {
            OutPut.UV.x = INIT_L;
        }

    }
	else if (0.5f > OutPut.UV.x)
	{
		OutPut.UV.x -= ATime;
        if (OutPut.UV.x < INIT_S)
        {
            OutPut.UV.x = INIT_S;
        }
    }
	
    if (0.5f < OutPut.UV.y)
    {
        OutPut.UV.y += ATime;
        if (OutPut.UV.y > INIT_L)
        {
            OutPut.UV.y = INIT_L;
        }
    }
    else if (0.5f > OutPut.UV.y)
    {
        OutPut.UV.y -= ATime;
        if (OutPut.UV.y < INIT_S)
        {
            OutPut.UV.y = INIT_S;
        }
    }
	
	return OutPut;
}

Texture2D ImageTexture : register(t0);
SamplerState ImageSampler : register(s0);

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
