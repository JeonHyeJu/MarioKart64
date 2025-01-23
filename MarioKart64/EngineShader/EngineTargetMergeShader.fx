struct EngineVertex
{
	float4 POSITION : POSITION;
	float4 UV : TEXCOORD;
};

struct VertexShaderOutPut
{
	float4 SVPOSITION : SV_POSITION;
	float4 UV : TEXCOORD;
};

VertexShaderOutPut TargetMerge_VS(EngineVertex _Vertex)
{
	VertexShaderOutPut OutPut;
	OutPut.SVPOSITION = _Vertex.POSITION;
	OutPut.UV = _Vertex.UV;
	return OutPut;
}
Texture2D MergeTex : register(t0);
SamplerState ImageSampler : register(s0);

float4 TargetMerge_PS(VertexShaderOutPut _Vertex) : SV_Target0
{
	float4 Color = MergeTex.Sample(ImageSampler, _Vertex.UV.xy);
	
	if (0.0f >= Color.a)
	{
		clip(-1);
	}
	
	// Color.a = max(Color.a, 1.0f);
	
	return Color;
};
