struct EngineVertex
{
	float4 POSITION : POSITION;
	float4 COLOR : COLOR;
};

struct VertexShaderOutPut
{
	float4 SVPOSITION : SV_POSITION;
	float4 NEWPOSITION : POSITION;
	float4 COLOR : COLOR;
};

cbuffer FTransform : register(b0)
{
	float4 Scale;
	float4 Rotation;
	float4 Location;

	float4x4 ScaleMat;
	float4x4 RotationMat;
	float4x4 LocationMat;
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
	float4x4 WVP;
};

VertexShaderOutPut VertexToWorld(EngineVertex _Vertex)
{
	VertexShaderOutPut OutPut;
	OutPut.SVPOSITION = mul(_Vertex.POSITION, WVP);
	OutPut.NEWPOSITION = _Vertex.POSITION;
	OutPut.COLOR = _Vertex.COLOR;
	
	return OutPut;
}

float4 PixelToWorld(VertexShaderOutPut _Vertex) : SV_Target0
{
	
	if (_Vertex.NEWPOSITION.x < 0)
	{
		return float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		return float4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	
	return _Vertex.COLOR;
}