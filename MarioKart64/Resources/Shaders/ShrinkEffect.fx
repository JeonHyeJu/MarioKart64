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
    const float INIT_S = -1.f;
    const float INIT_L = 2.f;
	
    if (OutPut.UV.x > 0.5f)
	{
		OutPut.UV.x += ATime;
        if (OutPut.UV.x > INIT_L)
        {
            OutPut.UV.x = INIT_L;
        }
    }
	
	
    if (OutPut.UV.y > 0.5f)
    {
        OutPut.UV.y += ATime;
        if (OutPut.UV.y > INIT_L)
        {
            OutPut.UV.y = INIT_L;
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
