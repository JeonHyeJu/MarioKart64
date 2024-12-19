struct EngineVertex
{
	float4 POSITION : POSITION;
	float4 COLOR : COLOR;
};

struct VertexShaderOutPut
{
	float4 SVPOSITION : SV_POSITION;
	float4 COLOR : COLOR;
};

VertexShaderOutPut VertexToWorld(EngineVertex _Vertex)
{
	VertexShaderOutPut OutPut;
	
	OutPut.SVPOSITION = _Vertex.POSITION;
	OutPut.COLOR = _Vertex.COLOR;
	
	return OutPut;
}

float4 PixelToWorld(VertexShaderOutPut _Vertex) : SV_Target0
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}